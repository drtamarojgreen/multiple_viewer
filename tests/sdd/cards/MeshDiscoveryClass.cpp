#include <iostream>
#include <future>
#include "analytics/mesh_discovery_engine.h"
#include "analytics/worker_pool.h"
#include "../cpp/util/fact_utils.h"

using namespace Sorrel::Sdd::Util;

// @Card: mesh_discovery_verification
// @Results mesh_discovery_operational == true
void mesh_discovery_card(const std::map<std::string, std::string>& facts) {
    analytics::WorkerPool pool(4);
    analytics::DiscoveryConfig cfg;
    cfg.maxLevels = 2;
    cfg.maxTotalTerms = 10;

    analytics::MeshDiscoveryEngine engine(pool, cfg);
    Graph g;

    auto future = engine.runDiscovery(g, "Alzheimer");
    future.wait();

    bool has_root = false;
    bool has_child = false;
    for (const auto& node : g.nodes) {
        if (node.label == "Alzheimer") has_root = true;
        if (node.label == "Amyloid" || node.label == "Tau") has_child = true;
    }

    bool operational = has_root && has_child && (g.nodes.size() > 1);
    std::cout << "mesh_discovery_operational = " << (operational ? "true" : "false") << std::endl;
}

int main() {
    auto facts = FactReader::readFacts("mesh_discovery.facts");
    if (facts.empty()) return 1;
    mesh_discovery_card(facts);
    return 0;
}
