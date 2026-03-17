#ifndef CHAI_CDD_UTIL_FACT_UTILS_H
#define CHAI_CDD_UTIL_FACT_UTILS_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

namespace Chai::Cdd::Util {

inline std::string trim(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) {
        start++;
    }
    if (start == s.end()) return "";
    auto end = s.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));
    return std::string(start, end + 1);
}

class FactReader {
public:
    static std::map<std::string, std::string> readFacts(const std::string& filepath) {
        std::map<std::string, std::string> facts;

        std::string target_path = filepath;
        if (!fs::exists(target_path)) {
            // Try relative to bin if execution is from tests/cdd
            if (filepath.find("../facts/") == 0) {
                std::string alt = "facts/" + filepath.substr(9);
                if (fs::exists(alt)) target_path = alt;
            } else if (filepath.find("facts/") == 0) {
                std::string alt = "../" + filepath;
                if (fs::exists(alt)) target_path = alt;
            }
        }

        std::ifstream file(target_path);
        if (!file.is_open()) {
             std::cerr << "Error: Could not read facts from " << filepath << " (checked: " << target_path << ")" << std::endl;
             std::cerr << "Current path: " << fs::current_path() << std::endl;
             return facts;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::string trimmed = trim(line);
            if (trimmed.empty() || trimmed[0] == '#' || trimmed.find("Situation:") == 0) continue;

            size_t space_pos = trimmed.find(' ');
            if (space_pos == std::string::npos) continue;

            std::string rest = trim(trimmed.substr(space_pos + 1));
            size_t eq_pos = rest.find('=');
            if (eq_pos != std::string::npos) {
                std::string key = trim(rest.substr(0, eq_pos));
                std::string value = trim(rest.substr(eq_pos + 1));
                facts[key] = value;
            }
        }
        return facts;
    }
};

} // namespace Chai::Cdd::Util

#endif // CHAI_CDD_UTIL_FACT_UTILS_H
