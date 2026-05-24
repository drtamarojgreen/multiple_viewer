#include "yaml_parser.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace io {

std::map<std::string, std::string> YamlParser::loadSimpleYaml(const std::string& filepath) {
    std::map<std::string, std::string> config;
    std::ifstream file(filepath);
    if (!file.is_open()) return config;

    std::string line;
    while (std::getline(file, line)) {
        // Remove comments
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos) line = line.substr(0, commentPos);

        // Trim
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        size_t last = line.find_last_not_of(" \t\r\n");
        if (last != std::string::npos) line = line.substr(0, last + 1);
        else continue;

        if (line.empty()) continue;

        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);

            // Trim key and value
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            if (!key.empty()) {
                config[key] = value;
            }
        }
    }
    return config;
}

std::string YamlParser::getValue(const std::map<std::string, std::string>& config, const std::string& key, const std::string& defaultVal) {
    auto it = config.find(key);
    return (it != config.end()) ? it->second : defaultVal;
}

int YamlParser::getInt(const std::map<std::string, std::string>& config, const std::string& key, int defaultVal) {
    std::string val = getValue(config, key);
    if (val.empty()) return defaultVal;
    try {
        return std::stoi(val);
    } catch (...) {
        return defaultVal;
    }
}

bool YamlParser::getBool(const std::map<std::string, std::string>& config, const std::string& key, bool defaultVal) {
    std::string val = getValue(config, key);
    if (val.empty()) return defaultVal;
    std::transform(val.begin(), val.end(), val.begin(), ::tolower);
    return (val == "true" || val == "yes" || val == "1");
}

float YamlParser::getFloat(const std::map<std::string, std::string>& config, const std::string& key, float defaultVal) {
    std::string val = getValue(config, key);
    if (val.empty()) return defaultVal;
    try {
        return std::stof(val);
    } catch (...) {
        return defaultVal;
    }
}

std::vector<std::string> YamlParser::getStringList(const std::map<std::string, std::string>& config, const std::string& key) {
    std::string val = getValue(config, key);
    std::vector<std::string> results;
    if (val.empty()) return results;

    std::stringstream ss(val);
    std::string item;
    while (std::getline(ss, item, ',')) {
        item.erase(0, item.find_first_not_of(" \t\""));
        size_t last = item.find_last_not_of(" \t\"");
        if (last != std::string::npos) item = item.substr(0, last + 1);
        if (!item.empty()) results.push_back(item);
    }
    return results;
}

} // namespace io
