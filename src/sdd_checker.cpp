#include "sdd_checker.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <ctime>

namespace fs = std::filesystem;

SddCheckResult SddChecker::checkRepository(const std::string& pathOrUrl) {
    std::string targetPath = pathOrUrl;
    bool wasCloned = false;

    if (isUrl(pathOrUrl)) {
        targetPath = cloneRepository(pathOrUrl);
        if (targetPath.empty()) {
            return {false, "Failed to clone repository: " + pathOrUrl, {}};
        }
        wasCloned = true;
    }

    fs::path repoPath(targetPath);
    if (!fs::exists(repoPath) || !fs::is_directory(repoPath)) {
        return {false, "Repository path does not exist or is not a directory: " + targetPath, {}};
    }

    SddCheckResult result = performChecks(repoPath);

    if (wasCloned) {
        std::error_code ec;
        fs::remove_all(repoPath, ec);
        if (ec) {
            std::cerr << "Warning: Failed to cleanup " << repoPath << ": " << ec.message() << std::endl;
        }
    }

    return result;
}

bool SddChecker::isUrl(const std::string& str) {
    return str.find("http://") == 0 || str.find("https://") == 0 || str.find("git@") == 0;
}

std::string SddChecker::cloneRepository(const std::string& url) {
    // Basic sanitization: only allow alphanumeric, dots, slashes, colons, underscores, hyphens, and at signs
    std::string sanitizedUrl = "";
    for (char c : url) {
        if (std::isalnum(c) || c == '.' || c == '/' || c == ':' || c == '_' || c == '-' || c == '@') {
            sanitizedUrl += c;
        } else {
            // Rejection of suspicious characters
            return "";
        }
    }

    if (sanitizedUrl.empty()) return "";

    std::string tmpDir = "tests/temp/cloned_repo_" + std::to_string(std::time(nullptr)) + "_" + std::to_string(std::rand());
    std::string cmd = "git clone --depth 1 " + sanitizedUrl + " " + tmpDir + " > /dev/null 2>&1";
    int ret = std::system(cmd.c_str());
    if (ret == 0) {
        return tmpDir;
    }
    return "";
}

SddCheckResult SddChecker::performChecks(const fs::path& repoPath) {
    SddCheckResult result;
    result.passed = true;
    result.message = "SDD Structure Check for " + repoPath.string();

    bool structureOk = checkDirectoryStructure(repoPath, result.details);
    bool filesOk = checkMandatoryFiles(repoPath, result.details);
    bool factsOk = checkFactsFormat(repoPath, result.details);
    bool cardsOk = checkCardsFormat(repoPath, result.details);
    bool checkoutOk = checkCheckoutArtifacts(repoPath, result.details);

    result.passed = structureOk && filesOk && factsOk && cardsOk && checkoutOk;

    if (result.passed) {
        result.message += " - PASSED";
    } else {
        result.message += " - FAILED";
    }

    return result;
}

bool SddChecker::checkDirectoryStructure(const fs::path& repoPath, std::vector<std::string>& details) {
    bool ok = true;
    std::vector<fs::path> requiredDirs = {
        repoPath / "tests/sdd",
        repoPath / "tests/sdd/facts",
        repoPath / "tests/sdd/cards"
    };

    for (const auto& dir : requiredDirs) {
        if (!fs::exists(dir) || !fs::is_directory(dir)) {
            details.push_back("Missing directory: " + fs::relative(dir, repoPath).string());
            ok = false;
        } else {
            details.push_back("Found directory: " + fs::relative(dir, repoPath).string());
        }
    }
    return ok;
}

bool SddChecker::checkFactsFormat(const fs::path& repoPath, std::vector<std::string>& details) {
    fs::path factsDir = repoPath / "tests/sdd/facts";
    if (!fs::exists(factsDir) || !fs::is_directory(factsDir)) return false;

    bool foundSituation = false;
    for (const auto& entry : fs::directory_iterator(factsDir)) {
        if (entry.path().extension() == ".facts" || entry.path().extension() == ".fact") {
            std::ifstream file(entry.path());
            std::string line;
            while (std::getline(file, line)) {
                if (line.find("Situation:") == 0) {
                    foundSituation = true;
                    break;
                }
            }
            if (foundSituation) break;
        }
    }

    if (!foundSituation) {
        details.push_back("No 'Situation:' header found in any .facts files.");
        return false;
    }
    details.push_back("Found 'Situation:' header in .facts files.");
    return true;
}

bool SddChecker::checkCardsFormat(const fs::path& repoPath, std::vector<std::string>& details) {
    fs::path cardsDir = repoPath / "tests/sdd/cards";
    if (!fs::exists(cardsDir) || !fs::is_directory(cardsDir)) return false;

    bool foundCardDecorator = false;
    for (const auto& entry : fs::recursive_directory_iterator(cardsDir)) {
        if (entry.path().extension() == ".cpp") {
            std::ifstream file(entry.path());
            std::string line;
            while (std::getline(file, line)) {
                if (line.find("// @Card:") != std::string::npos) {
                    foundCardDecorator = true;
                    break;
                }
            }
            if (foundCardDecorator) break;
        }
    }

    if (!foundCardDecorator) {
        details.push_back("No '// @Card:' decorator found in any .cpp files in cards/ directory.");
        return false;
    }
    details.push_back("Found '// @Card:' decorator in cards/ directory.");
    return true;
}

bool SddChecker::checkCheckoutArtifacts(const fs::path& repoPath, std::vector<std::string>& details) {
    fs::path checkoutFile = repoPath / "tests/sdd/sorrel_checkouts.md";
    if (!fs::exists(checkoutFile)) {
        checkoutFile = repoPath / "tests/sdd/chai_checkouts.md";
    }
    if (!fs::exists(checkoutFile)) return false;

    std::ifstream file(checkoutFile);
    std::string line;
    bool foundArtifacts = false;
    while (std::getline(file, line)) {
        if (line.find("Artifacts:") != std::string::npos || line.find("*Artifacts:*") != std::string::npos) {
            foundArtifacts = true;
            break;
        }
    }

    if (!foundArtifacts) {
        details.push_back("Mandatory file sorrel_checkouts.md does not contain 'Artifacts:' mentions.");
        return false;
    }
    details.push_back("Found 'Artifacts:' mentions in sorrel_checkouts.md.");
    return true;
}

bool SddChecker::checkMandatoryFiles(const fs::path& repoPath, std::vector<std::string>& details) {
    bool ok = true;

    auto checkOne = [&](const std::string& name1, const std::string& name2) {
        fs::path p1 = repoPath / "tests/sdd" / name1;
        fs::path p2 = repoPath / "tests/sdd" / name2;
        if (fs::exists(p1) && fs::is_regular_file(p1)) {
            details.push_back("Found mandatory file: " + name1);
            return true;
        } else if (fs::exists(p2) && fs::is_regular_file(p2)) {
            details.push_back("Found mandatory file: " + name2);
            return true;
        } else {
            details.push_back("Missing mandatory file: " + name1 + " or " + name2);
            return false;
        }
    };

    bool checkinsOk = checkOne("sorrel_checkins.md", "chai_checkins.md");
    bool checkoutsOk = checkOne("sorrel_checkouts.md", "chai_checkouts.md");

    return checkinsOk && checkoutsOk;
}
