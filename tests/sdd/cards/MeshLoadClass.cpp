#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>
#include "io/io_manager.h"
#include "../cpp/util/fact_utils.h"

namespace fs = std::filesystem;
using namespace Sorrel::Sdd::Util;

// @Card: mesh_json_load
// @Results mesh_json_load_operational == true
void mesh_json_load_card(const std::map<std::string, std::string>& facts) {
    fs::create_directories("../../tests/temp/");
    std::string test_file = "../../tests/temp/mesh_test.json";

    std::ofstream out(test_file);
    out << "{\n";
    out << "  \"nodes\": [\n";
    out << "    {\"label\": \"" << facts.at("mesh_node_label") << "\", \"id\": " << facts.at("mesh_node_id") << ", \"weight\": 10, \"subjectIndex\": 1}\n";
    out << "  ],\n";
    out << "  \"edges\": [\n";
    out << "    {\"source\": " << facts.at("mesh_edge_source") << ", \"target\": " << facts.at("mesh_edge_target") << "}\n";
    out << "  ]\n";
    out << "}\n";
    out.close();

    Graph g;
    bool loaded = io::IOManager::loadMeshJSON(g, test_file);

    int expected_id = std::stoi(facts.at("mesh_node_id"));
    std::string expected_label = facts.at("mesh_node_label");
    if (!expected_label.empty() && expected_label.front() == '"') expected_label = expected_label.substr(1, expected_label.size()-2);

    bool node_ok = g.nodeExists(expected_id) && g.nodeMap.at(expected_id).label == expected_label;
    bool weight_ok = g.nodeExists(expected_id) && g.nodeMap.at(expected_id).weight == 10;

    bool operational = loaded && node_ok && weight_ok;

    std::cout << "mesh_json_load_operational = " << (operational ? "true" : "false") << std::endl;

    if (fs::exists(test_file)) fs::remove(test_file);
}

int main() {
    auto facts = FactReader::readFacts("mesh_load.facts");
    if (facts.empty()) return 1;
    mesh_json_load_card(facts);
    return 0;
}
