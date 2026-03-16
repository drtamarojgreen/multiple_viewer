#include <iostream>
#include <fstream>
#include <string>
#include <filesystem> // Required for C++17 filesystem operations
#include <map>
#include <cstdio>
#include <memory>
#include <array>
#include <chrono> // For timestamping reports
#include <iomanip> // For formatting timestamp
#include "cpp/util/string_utils.h" // Include the new utility header
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

// Function to find the 'cdd' directory based on the executable's path
fs::path get_executable_cdd_directory(const char* argv0) {
    fs::path executable_path = fs::canonical(argv0); // Get absolute path to executable
    fs::path executable_parent_path = executable_path.parent_path(); // Should be tests/chai/cdd/

    // Assuming card_runner is directly inside tests/chai/cdd/
    if (fs::exists(executable_parent_path) && fs::is_directory(executable_parent_path)) {
        return executable_parent_path;
    }
    return {}; // Return empty path if not found
}

// Function to execute a command and return its output
std::string execute_command(const std::string& cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        return "ERROR: popen failed";
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int main(int argc, char* argv[]) {
    std::cout << "CHAI card runner" << std::endl;

    fs::path cdd_dir = get_executable_cdd_directory(argv[0]);
    if (cdd_dir.empty()) {
        std::cerr << "Error: 'cdd' directory not found from executable path." << std::endl;
        return 1;
    }

    // Prepare report directory and file
    fs::path reports_dir = cdd_dir / "reports";
    if (!fs::exists(reports_dir)) {
        fs::create_directories(reports_dir);
    }

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d_%H%M%S");
    fs::path report_file_path = reports_dir / ("run_" + ss.str() + ".report");
    std::ofstream report_file(report_file_path);

    // Helper to print and log
    auto log = [&](const std::string& msg, bool to_cerr = false) {
        if (to_cerr) std::cerr << msg << std::endl;
        else std::cout << msg << std::endl;
        if (report_file.is_open()) report_file << msg << std::endl;
    };

    log("CHAI card runner");

    if (cdd_dir.empty()) {
        log("Error: 'cdd' directory not found from executable path.", true);
        return 1;
    }

    fs::path facts_file_path;
    if (argc > 1) {
        facts_file_path = fs::path(argv[1]);
        if (facts_file_path.is_relative()) {
            facts_file_path = cdd_dir / "facts" / facts_file_path;
        }
    } else {
        facts_file_path = cdd_dir / "facts" / "environment.facts";
    }

    log("Attempting to read facts from: " + facts_file_path.string());

    // situation -> level -> key -> value
    std::map<std::string, std::map<std::string, std::map<std::string, std::string>>> indexed_facts;
    std::string current_situation = "Default";

    std::ifstream facts_file(facts_file_path);
    if (facts_file.is_open()) {
        std::string line;
        while (std::getline(facts_file, line)) {
            std::string trimmed_line = Chai::Cpp::Util::trim(line);

            if (trimmed_line.empty()) continue;

            if (trimmed_line.find("Situation:") == 0) {
                current_situation = Chai::Cpp::Util::trim(trimmed_line.substr(10));
                log("Switching to Situation: [" + current_situation + "]");
                continue;
            }

            if (trimmed_line[0] == '#') {
                continue;
            }

            size_t level_end = trimmed_line.find(' ');
            if (level_end != std::string::npos) {
                std::string level = trimmed_line.substr(0, level_end);
                std::string rest = Chai::Cpp::Util::trim(trimmed_line.substr(level_end + 1));
                size_t eq_pos = rest.find('=');
                if (eq_pos != std::string::npos) {
                    std::string key = Chai::Cpp::Util::trim(rest.substr(0, eq_pos));
                    std::string value = Chai::Cpp::Util::trim(rest.substr(eq_pos + 1));
                    indexed_facts[current_situation][level][key] = value;
                }
            }
        }
        facts_file.close();
    } else {
        log("Error: Could not open facts file: " + facts_file_path.string(), true);
        return 1;
    }

    for (const auto& [situation, levels] : indexed_facts) {
        log("--- Situation: [" + situation + "] ---");
        for (const std::string& level : {"Is", "Needs", "Results"}) {
            if (levels.count(level)) {
                log("  --- Level: [" + level + "] ---");
                for (const auto& [key, value] : levels.at(level)) {
                    log("  Fact [" + level + "] - " + key + ": " + value);
                }
            }
        }
        log("--- End Situation: [" + situation + "] ---");
    }

    struct LogicalCard {
        std::string name;
        std::string className;
        fs::path executablePath;
        std::map<std::string, std::string> decorators;
    };

    std::vector<LogicalCard> discovered_cards;

    fs::path cards_dir = cdd_dir / "cards";
    if (fs::exists(cards_dir) && fs::is_directory(cards_dir)) {
        for (const auto& entry : fs::recursive_directory_iterator(cards_dir)) {
            if (fs::is_regular_file(entry)) {
                std::string filename = entry.path().filename().string();
                // Simple heuristic: look for executables matching source files
                if (filename.find('.') == std::string::npos) {
                    fs::path source_path = entry.path().string() + ".cpp";
                    if (fs::exists(source_path)) {
                        fs::path relative_path = fs::relative(entry.path(), cards_dir);
                        std::string class_name = relative_path.parent_path().string();
                        if (class_name.empty()) class_name = filename; // The file name is the class

                        std::ifstream source_file(source_path);
                        std::string source_line;
                        LogicalCard* current_card = nullptr;

                        std::map<std::string, std::string> legacy_decorators;
                        while (std::getline(source_file, source_line)) {
                            std::string trimmed_source = Chai::Cpp::Util::trim(source_line);
                            if (trimmed_source.find("// @Card:") == 0) {
                                std::string card_name = Chai::Cpp::Util::trim(trimmed_source.substr(9));
                                discovered_cards.push_back({card_name, class_name, entry.path(), {}});
                                current_card = &discovered_cards.back();
                            } else if (trimmed_source.find("// @") == 0) {
                                std::string dec_line = trimmed_source.substr(4);
                                size_t space_pos = dec_line.find(' ');
                                std::string key, value;
                                if (space_pos != std::string::npos) {
                                    key = dec_line.substr(0, space_pos);
                                    value = dec_line.substr(space_pos + 1);
                                } else {
                                    key = dec_line;
                                    value = "true";
                                }

                                if (current_card) {
                                    current_card->decorators[key] = value;
                                } else {
                                    legacy_decorators[key] = value;
                                }
                            } else if (!trimmed_source.empty() && trimmed_source.find("//") != 0 && current_card) {
                                current_card = nullptr;
                            }
                        }

                        if (discovered_cards.empty()) {
                            discovered_cards.push_back({filename, class_name, entry.path(), legacy_decorators});
                        }
                    }
                }
            }
        }
    } else {
        log("(no 'cards' directory found)");
    }

    log("--- Discovered & Executed Cards ---");
    for (const auto& card : discovered_cards) {
        std::string situation = card.decorators.count("Situation") ? card.decorators.at("Situation") : "Default";
        log("Class: [" + card.className + "] Card: [" + card.name + "] Situation: [" + situation + "]");

        if (indexed_facts.find(situation) == indexed_facts.end()) {
            log("  Skipping: Situation [" + situation + "] not found in fact files.");
            continue;
        }

        const auto& active_facts = indexed_facts.at(situation);

        for (const auto& [key, value] : card.decorators) {
            log("  Decorator: " + key + " = " + value);
        }

        bool should_execute = true;

        // Is Level Evaluation
        if (card.decorators.count("Is")) {
            std::string val = card.decorators.at("Is");
            size_t eq_pos = val.find("==");
            if (eq_pos != std::string::npos) {
                std::string key = Chai::Cpp::Util::trim(val.substr(0, eq_pos));
                std::string expected = Chai::Cpp::Util::trim(val.substr(eq_pos + 2));
                log("  Level: [Is] - Expected: " + key + " == " + expected);
                if (active_facts.count("Is") && active_facts.at("Is").count(key) && active_facts.at("Is").at(key) == expected) {
                    log("    Match: Verified system state.");
                } else {
                    std::string current_val = (active_facts.count("Is") && active_facts.at("Is").count(key)) ? active_facts.at("Is").at(key) : "NOT_FOUND";
                    log("    Violation: System state mismatch! (Found: " + current_val + ")");
                    should_execute = false;
                }
            }
        }

        if (should_execute && card.decorators.count("platform")) {
            std::string platform_req = card.decorators.at("platform");
            if (active_facts.count("Is") && active_facts.at("Is").count("platform") && platform_req != active_facts.at("Is").at("platform")) {
                log("  Skipping: Platform mismatch (Required: " + platform_req
                          + ", Current: " + active_facts.at("Is").at("platform") + ")");
                should_execute = false;
            }
        }

        // Needs Level Evaluation
        if (should_execute && card.decorators.count("Needs")) {
            std::string val = card.decorators.at("Needs");
            size_t eq_pos = val.find("==");
            if (eq_pos != std::string::npos) {
                std::string key = Chai::Cpp::Util::trim(val.substr(0, eq_pos));
                std::string expected = Chai::Cpp::Util::trim(val.substr(eq_pos + 2));
                log("  Level: [Needs] - Prerequisite: " + key + " == " + expected);
                bool met = false;
                if (active_facts.count("Needs") && active_facts.at("Needs").count(key) && active_facts.at("Needs").at(key) == expected) {
                    log("    Requirement met: " + key + " == " + expected);
                    met = true;
                } else if (active_facts.count("Is") && active_facts.at("Is").count(key) && active_facts.at("Is").at(key) == expected) {
                    log("    Requirement met (verified system state): " + key + " == " + expected);
                    met = true;
                }

                if (!met) {
                    log("    Skipping: Prerequisite not met (" + key + " == " + expected + ")");
                    should_execute = false;
                }
            }
        }

        if (should_execute && card.decorators.count("requires")) {
            std::string req = card.decorators.at("requires");
            size_t eq_pos = req.find("==");
            if (eq_pos != std::string::npos) {
                std::string key = Chai::Cpp::Util::trim(req.substr(0, eq_pos));
                std::string value = Chai::Cpp::Util::trim(req.substr(eq_pos + 2));
                if (!active_facts.count("Is") || !active_facts.at("Is").count(key) || active_facts.at("Is").at(key) != value) {
                    log("  Skipping: Requirement not met (" + key + " == " + value + ")");
                    should_execute = false;
                } else {
                    log("  Requirement met: " + key + " == " + value);
                }
            }
        }

        if (should_execute) {
            std::string output = execute_command(card.executablePath.string() + " " + card.name);
            log("Resulting Output Trace:\n" + output);

            log("  Level: [Results] (Observations)");
            std::stringstream output_stream(output);
            std::string out_line;
            std::map<std::string, std::string> observed_results;
            while (std::getline(output_stream, out_line)) {
                std::string trimmed_out = Chai::Cpp::Util::trim(out_line);
                size_t eq_pos = trimmed_out.find('=');
                if (eq_pos != std::string::npos) {
                    std::string key = Chai::Cpp::Util::trim(trimmed_out.substr(0, eq_pos));
                    std::string value = Chai::Cpp::Util::trim(trimmed_out.substr(eq_pos + 1));
                    observed_results[key] = value;
                }
            }

            if (card.decorators.count("Results")) {
                std::string val = card.decorators.at("Results");
                size_t eq_pos = val.find("==");
                if (eq_pos != std::string::npos) {
                    std::string key = Chai::Cpp::Util::trim(val.substr(0, eq_pos));
                    std::string expected = Chai::Cpp::Util::trim(val.substr(eq_pos + 2));
                    if (observed_results.count(key) && observed_results[key] == expected) {
                        log("    Observed Result: " + key + " == " + expected + " (Verified)");
                    } else {
                        log("    Missing/Mismatch Result: " + key + " == " + expected + " (Found: " + (observed_results.count(key) ? observed_results[key] : "NOT_FOUND") + ")");
                    }
                }
            } else {
                for (const auto& [key, value] : observed_results) {
                    log("    Observed Fact: " + key + " == " + value);
                }
            }
        }
        log("");
    }

    return 0;
}
