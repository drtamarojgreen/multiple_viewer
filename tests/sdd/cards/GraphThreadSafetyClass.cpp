#include <iostream>
#include <vector>
#include <thread>
#include "map_logic.h"
#include "../cpp/util/fact_utils.h"

using namespace Sorrel::Sdd::Util;

// @Card: graph_thread_safety_verification
// @Results graph_thread_safety_operational == true
void graph_thread_safety_card(const std::map<std::string, std::string>& facts) {
    Graph g;
    const int num_threads = 4;
    const int nodes_per_thread = 100;
    std::vector<std::thread> threads;

    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([&g, t, nodes_per_thread]() {
            for (int i = 0; i < nodes_per_thread; ++i) {
                int idx = t * nodes_per_thread + i;
                g.addNode(GraphNode("Node " + std::to_string(idx), idx));
            }
        });
    }

    for (auto& th : threads) th.join();

    bool operational = (g.nodes.size() == static_cast<size_t>(num_threads * nodes_per_thread));
    std::cout << "graph_thread_safety_operational = " << (operational ? "true" : "false") << std::endl;
}

int main() {
    auto facts = FactReader::readFacts("environment.facts");
    if (facts.empty()) return 1;
    graph_thread_safety_card(facts);
    return 0;
}
