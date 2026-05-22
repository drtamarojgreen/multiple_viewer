#ifndef YAML_PARSER_H
#define YAML_PARSER_H

#include <string>
#include <map>
#include <vector>

namespace io {

class YamlParser {
public:
    static std::map<std::string, std::string> loadSimpleYaml(const std::string& filepath);

    // Internal helper to parse key-value pairs ignoring nesting for now
    static std::string getValue(const std::map<std::string, std::string>& config, const std::string& key, const std::string& defaultVal = "");
    static int getInt(const std::map<std::string, std::string>& config, const std::string& key, int defaultVal = 0);
    static bool getBool(const std::map<std::string, std::string>& config, const std::string& key, bool defaultVal = false);
    static float getFloat(const std::map<std::string, std::string>& config, const std::string& key, float defaultVal = 0.0f);
};

} // namespace io

#endif // YAML_PARSER_H
