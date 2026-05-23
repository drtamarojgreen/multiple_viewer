#include "nlp_engine.h"
#include "../io/yaml_parser.h"
#include <algorithm>
#include <sstream>

namespace analytics {

static std::set<std::string> g_stopWords;
static std::vector<std::string> g_suffixes;

void NlpEngine::loadConfig(const std::string& filepath) {
    auto cfg = io::YamlParser::loadSimpleYaml(filepath);
    auto sw = io::YamlParser::getStringList(cfg, "stop_words");
    g_stopWords = std::set<std::string>(sw.begin(), sw.end());
    g_suffixes = io::YamlParser::getStringList(cfg, "ner_suffixes");
}

std::set<std::string> NlpEngine::extractKeywords(const std::vector<std::string>& texts) {
    std::set<std::string> keywords;

    for (const auto& text : texts) {
        std::stringstream ss(text);
        std::string word;
        while (ss >> word) {
            std::string clean;
            for (char c : word) if (std::isalnum(c)) clean += std::tolower(c);
            if (clean.size() > 4 && g_stopWords.find(clean) == g_stopWords.end()) {
                keywords.insert(clean);
            }
        }
    }
    return keywords;
}

std::set<std::string> NlpEngine::detectEntities(const std::vector<std::string>& texts) {
    std::set<std::string> entities;

    for (const auto& text : texts) {
        std::stringstream ss(text);
        std::string word;
        while (ss >> word) {
            std::string clean;
            for (char c : word) if (std::isalnum(c)) clean += std::tolower(c);
            for (const auto& s : g_suffixes) {
                if (clean.size() > 5 && clean.size() >= s.size() &&
                    clean.compare(clean.size() - s.size(), s.size(), s) == 0) {
                    entities.insert(word);
                    break;
                }
            }
        }
    }
    return entities;
}

} // namespace analytics
