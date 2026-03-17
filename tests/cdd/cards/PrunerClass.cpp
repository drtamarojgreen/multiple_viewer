#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "analytics/analytics_engine_ext.h"
#include "../cpp/util/fact_utils.h"

using namespace Chai::Cdd::Util;
using namespace analytics;

// @Card: pruner_logic_verification
// @Results pruner_logic_operational == true
void pruner_logic_verification_card(const std::map<std::string, std::string>& facts) {
    Graph g;
    // Create a simple star graph: 0 is center, 1,2,3 are leaves
    g.addNode(GraphNode("Center", 0));
    g.addNode(GraphNode("Leaf 1", 1));
    g.addNode(GraphNode("Leaf 2", 2));
    g.addNode(GraphNode("Leaf 3", 3));
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(0, 3);

    CentralityMetrics metrics = AnalyticsEngine::computeCentrality(g);

    // Node 0 should have the highest degree centrality
    float center_dc = metrics.degreeCentrality[0];
    float leaf_dc = metrics.degreeCentrality[1];

    bool operational = (center_dc > leaf_dc);

    std::cout << "pruner_logic_operational = " << (operational ? "true" : "false") << std::endl;
}

int main() {
    auto facts = FactReader::readFacts("../facts/pruner.facts");
    if (facts.empty()) {
        std::cerr << "Error: Could not read facts from ../facts/pruner.facts" << std::endl;
        return 1;
    }
    pruner_logic_verification_card(facts);
    return 0;
}
