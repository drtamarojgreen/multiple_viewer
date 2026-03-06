#include "bdd_runner.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <memory>

namespace bdd {

void BDDRunner::registerStep(const std::string& pattern, StepFunc func) {
    steps_.push_back({std::regex(pattern), func});
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

    if (!found) return true;

    std::cout << "      [STEP] " << action << std::endl;

    for (const auto& step : steps_) {
        std::smatch match;
        if (std::regex_match(action, match, step.first)) {
            std::vector<std::string> args;
            for (size_t i = 1; i < (size_t)match.size(); ++i) {
                args.push_back(match[i].str());
            }
            try {
                step.second(ctx, args);
                if (!ctx.success) {
                    std::cerr << "      [FAIL] " << action << std::endl;
                    return false;
                }
                return true;
            } catch (const std::exception& e) {
                std::cerr << "      [EXCP] " << action << " : " << e.what() << std::endl;
                ctx.success = false;
                return false;
            } catch (...) {
                std::cerr << "      [EXCP] " << action << " : unknown" << std::endl;
                ctx.success = false;
                return false;
            }
        }
    }

    std::cerr << "      [MISS] " << action << std::endl;
    ctx.success = false;
    return false;
}

bool BDDRunner::runFeature(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "[BDD] Could not open feature file: " << filepath << "\n";
        return false;
    }

    std::string line;
    std::cout << "[BDD] Running feature: " << filepath << "\n";
    
    std::unique_ptr<BDDContext> ctx = nullptr;
    bool featureSuccess = true;

    while (std::getline(file, line)) {
        std::string t = trim(line);
        if (t.empty()) continue;

        if (t.compare(0, 8, "Feature:") == 0) {
            std::cout << "  " << t << "\n";
            continue;
        }

        if (t.compare(0, 9, "Scenario:") == 0) {
            std::cout << "    " << t << "\n";
            ctx = std::make_unique<BDDContext>();
            continue;
        }
        
        if (ctx) {
            // std::cout << "DEBUG: line=[" << line << "]" << std::endl;
            if (!executeLine(*ctx, line)) {
                featureSuccess = false;
                // Skip rest of scenario
                while (std::getline(file, line)) {
                    t = trim(line);
                    if (t.compare(0, 9, "Scenario:") == 0) {
                        std::cout << "    " << t << "\n";
                        ctx = std::make_unique<BDDContext>();
                        break;
                    }
                }
            }
        }
    }
    
    return featureSuccess;
}

} // namespace bdd
