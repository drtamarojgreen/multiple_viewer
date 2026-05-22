#include "nlp_engine.h"
#include <algorithm>
#include <sstream>

namespace analytics {

std::set<std::string> NlpEngine::extractKeywords(const std::vector<std::string>& texts) {
    std::set<std::string> keywords;
    std::set<std::string> stopWords = {"the", "and", "for", "with", "from", "that"};

    for (const auto& text : texts) {
        std::stringstream ss(text);
        std::string word;
        while (ss >> word) {
            std::string clean;
            for (char c : word) if (std::isalnum(c)) clean += std::tolower(c);
            if (clean.size() > 4 && stopWords.find(clean) == stopWords.end()) {
                keywords.insert(clean);
            }
        }
    }
    return keywords;
}

std::set<std::string> NlpEngine::detectEntities(const std::vector<std::string>& texts) {
    std::set<std::string> entities;
    std::vector<std::string> suffixes = {"mab", "nib", "vir", "afil", "stat"};

    for (const auto& text : texts) {
        std::stringstream ss(text);
        std::string word;
        while (ss >> word) {
            std::string clean;
            for (char c : word) if (std::isalnum(c)) clean += std::tolower(c);
            for (const auto& s : suffixes) {
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
