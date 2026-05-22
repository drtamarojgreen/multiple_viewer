#ifndef NLP_ENGINE_H
#define NLP_ENGINE_H

#include <string>
#include <vector>
#include <set>

namespace analytics {

class NlpEngine {
public:
    static std::set<std::string> extractKeywords(const std::vector<std::string>& texts);
    static std::set<std::string> detectEntities(const std::vector<std::string>& texts);
};

} // namespace analytics

#endif // NLP_ENGINE_H
