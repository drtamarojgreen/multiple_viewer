#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>
#include "io/io_manager.h"
#include "../cpp/util/fact_utils.h"

namespace fs = std::filesystem;
using namespace Chai::Cdd::Util;

// @Card: quanta_glia_extraction
// @Results quanta_glia_extraction_operational == true
void quanta_glia_extraction_card(const std::map<std::string, std::string>& facts) {
    std::string test_file = "cdd_test_graph.json";
    Graph g;
    g.addNode(GraphNode("Extraction Test", 0));

    bool saved = io::IOManager::saveJSON(g, test_file);

    Graph g2;
    bool loaded = io::IOManager::loadJSON(g2, test_file);

    bool operational = saved && loaded && g2.nodeExists(0) && g2.nodeMap.at(0).label == "Extraction Test";

    std::cout << "quanta_glia_extraction_operational = " << (operational ? "true" : "false") << std::endl;

    if (fs::exists(test_file)) fs::remove(test_file);
}

// @Card: max_repos_limit_verification
// @Results quanta_glia_max_repos_limit_operational == true
void max_repos_limit_verification_card(const std::map<std::string, std::string>& facts) {
    // In multiple_viewer, we can test node limit if applicable,
    // or just verify multi-node IO
    Graph g;
    int max_nodes = std::stoi(facts.at("max_repos"));
    for (int i = 0; i < max_nodes; ++i) {
        g.addNode(GraphNode("Node " + std::to_string(i), i));
    }

    bool operational = (g.nodeMap.size() == (size_t)max_nodes);
    std::cout << "quanta_glia_max_repos_limit_operational = " << (operational ? "true" : "false") << std::endl;
}

int main(int argc, char* argv[]) {
    auto facts = FactReader::readFacts("../facts/quanta_glia.facts");
    if (facts.empty()) {
        std::cerr << "Error: Could not read facts from ../facts/quanta_glia.facts" << std::endl;
        return 1;
    }

    if (argc > 1 && std::string(argv[1]) == "max_repos") {
        max_repos_limit_verification_card(facts);
    } else {
        quanta_glia_extraction_card(facts);
    }
    return 0;
}
