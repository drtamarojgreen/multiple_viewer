#include <iostream>
#include <map>
#include <memory>
#include "model/overlay_manager.h"
#include "map_logic.h"
#include "../cpp/util/fact_utils.h"

using namespace Chai::Cdd::Util;

// @Card: overlay_matching_verification
// @Results overlay_matching_operational == true
void overlay_matching_card(const std::map<std::string, std::string>& facts) {
    Graph network;
    network.addNode(GraphNode("Match", 1));

    Graph overlay;
    overlay.addNode(GraphNode("Match", 10));

    model::OverlayManager mgr(&network);
    mgr.addOverlay(1, &overlay);

    int matches = mgr.buildMatches("cdd_overlay_test.bin");
    bool operational = (matches == 1 && mgr.isOverlayNode(1));

    std::cout << "overlay_matching_operational = " << (operational ? "true" : "false") << std::endl;
}

int main(int argc, char* argv[]) {
    auto facts = FactReader::readFacts("overlay.facts");
    overlay_matching_card(facts);
    return 0;
}
