#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "search_logic.h"
#include "../cpp/util/fact_utils.h"

using namespace Chai::Cdd::Util;

// @Card: keyword_validator_verification
// @Results keyword_validator_operational == true
void keyword_validator_verification_card(const std::map<std::string, std::string>& facts) {
    std::string keyword = facts.at("keyword_target");

    Graph g;
    g.addNode(GraphNode("The quick brown fox", 0));
    g.addNode(GraphNode("Jumped over the dog", 1));

    std::vector<int> results = findSimilarTopics(g, keyword);

    bool operational = (results.size() == 1 && results[0] == 0);
    std::cout << "keyword_validator_operational = " << (operational ? "true" : "false") << std::endl;
}

// @Card: citation_validator_verification
// @Results citation_validator_operational == true
void citation_validator_verification_card(const std::map<std::string, std::string>& facts) {
    // For this context, let's say "citation" means searching for brackets like [1]
    Graph g;
    g.addNode(GraphNode("See ref [1] and [2].", 0));
    g.addNode(GraphNode("No citations here.", 1));

    std::vector<int> results = findSimilarTopics(g, "[");

    bool operational = (results.size() == 1 && results[0] == 0);
    std::cout << "citation_validator_operational = " << (operational ? "true" : "false") << std::endl;
}

int main(int argc, char* argv[]) {
    auto facts = FactReader::readFacts("evaluators.facts");
    if (facts.empty()) {
        return 1;
    }

    if (argc > 1 && std::string(argv[1]) == "citation") {
        citation_validator_verification_card(facts);
    } else {
        keyword_validator_verification_card(facts);
    }
    return 0;
}
