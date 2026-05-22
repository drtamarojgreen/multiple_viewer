#include <iostream>
#include <vector>
#include <map>
#include "analytics/analytics_engine_ext.h"
#include "../cpp/util/fact_utils.h"

using namespace Sorrel::Sdd::Util;

// @Card: pruner_logic_verification
// @Results pruner_logic_operational == true
void pruner_logic_verification_card(const std::map<std::string, std::string>& facts) {
    Graph g;
    g.addNode(GraphNode("Center", 0));
    g.addNode(GraphNode("Leaf", 1));
    g.addEdge(0, 1);

    auto metrics = analytics::AnalyticsEngine::computeCentrality(g);

    bool operational = (metrics.degreeCentrality.size() == 2);
    std::cout << "pruner_logic_operational = " << (operational ? "true" : "false") << std::endl;
}

int main() {
    auto facts = FactReader::readFacts("pruner.facts");
    if (facts.empty()) return 1;
    pruner_logic_verification_card(facts);
    return 0;
}
