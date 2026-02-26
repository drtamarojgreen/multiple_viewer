#include "bdd_runner.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>

namespace bdd {

void BDDRunner::registerStep(const std::string& pattern, StepFunc func) {
    steps_[pattern] = func;
}

std::string BDDRunner::trim(const std::string& s) {
    auto first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    auto last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, (last - first + 1));
}

bool BDDRunner::executeLine(BDDContext& ctx, const std::string& line) {
    std::string trimmed = trim(line);
    if (trimmed.empty() || trimmed[0] == '#') return true;

    // Remove Gherkin keywords
    std::vector<std::string> keywords = {"Given ", "When ", "Then ", "And ", "But "};
    std::string action = "";
    bool found = false;
    for (const auto& kw : keywords) {
        if (trimmed.compare(0, kw.length(), kw) == 0) {
            action = trim(trimmed.substr(kw.length()));
            found = true;
            break;
        }
    }

    if (!found) return true; // Ignore lines that don't start with a keyword (descriptions, etc.)

    // Very primitive matcher: exact match or placeholder match
    // For now, we look for matches that might contain quoted strings as arguments
    for (const auto& [pattern, func] : steps_) {
        // Simple regex-based matching of placeholders in quotes "..."
        // Pattern: "I have a node named \"(.*)\""
        std::regex reg(pattern);
        std::smatch match;
        if (std::regex_match(action, match, reg)) {
            std::vector<std::string> args;
            for (size_t i = 1; i < match.size(); ++i) {
                args.push_back(match[i].str());
            }
            try {
                func(ctx, args);
                return true;
            } catch (const std::exception& e) {
                std::cerr << "[BDD ERROR] Step failed: " << action << " - " << e.what() << "\n";
                return false;
            }
        }
    }

    std::cerr << "[BDD WARNING] No step match found for: " << action << "\n";
    return false;
}

bool BDDRunner::runFeature(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "[BDD] Could not open feature file: " << filepath << "\n";
        return false;
    }

    BDDContext ctx;
    std::string line;
    std::cout << "[BDD] Running feature: " << filepath << "\n";
    
    bool featureSuccess = true;
    while (std::getline(file, line)) {
        std::string t = trim(line);
        if (t.compare(0, 8, "Feature:") == 0) {
            std::cout << "  " << t << "\n";
            continue;
        }
        if (t.compare(0, 9, "Scenario:") == 0) {
            std::cout << "    " << t << "\n";
            ctx = BDDContext(); // Reset context for each scenario
            continue;
        }
        
        if (!executeLine(ctx, line)) {
            featureSuccess = false;
        }
    }
    
    return featureSuccess;
}

} // namespace bdd
