#include <iostream>
#include <map>
#include "layout/layout_manager.h"
#include "../cpp/util/fact_utils.h"

using namespace Chai::Cdd::Util;
using namespace layout;

// @Card: layout_circular_verification
// @Results layout_circular_operational == true
void layout_circular_card(const std::map<std::string, std::string>& facts) {
    Graph g;
    g.addNode(GraphNode("A", 0));
    g.addNode(GraphNode("B", 1));

    LayoutManager::applyCircular(g);

    // In a circular layout of 2 nodes, they should be at some distance from each other
    Point3D p0 = g.nodePos[0];
    Point3D p1 = g.nodePos[1];

    bool operational = (p0.x != p1.x || p0.y != p1.y);
    std::cout << "layout_circular_operational = " << (operational ? "true" : "false") << std::endl;
}

int main() {
    auto facts = FactReader::readFacts("tests/cdd/facts/layout.facts");
    layout_circular_card(facts);
    return 0;
}
