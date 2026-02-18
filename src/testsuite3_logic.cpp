#include "testsuite3_logic.h"
#include "map_logic.h"
#include "testsuite2_logic.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>
#include <string>
#include <vector>
#include <map>

namespace fs = std::filesystem;

void constructGraphFromSource(Graph& graph) {
    int nodeIdx = 0;
    int subjectIdx = 0;
    std::map<std::string, int> fileToSubject;

    // Regex for basic function matching
    std::regex funcRegex(R"((?:void|int|bool|double|float|std::vector|std::string)\s+([a-zA-Z0-9_:]+)\s*\([^)]*\)\s*\{)");

    for (const auto& entry : fs::directory_iterator("src")) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            if (filename.find(".cpp") != std::string::npos || filename.find(".h") != std::string::npos) {
                fileToSubject[filename] = subjectIdx++;

                std::ifstream file(entry.path());
                std::string line;
                while (std::getline(file, line)) {
                    std::smatch match;
                    if (std::regex_search(line, match, funcRegex)) {
                        std::string funcName = match[1];
                        GraphNode node(funcName, nodeIdx++, {}, 1, fileToSubject[filename]);
                        graph.addNode(node);
                    }
                }
            }
        }
    }

    // Simple heuristic for edges: if a function name appears in another file, add an edge
    for (auto& nodeA : graph.nodes) {
        for (const auto& entry : fs::directory_iterator("src")) {
             if (entry.is_regular_file()) {
                 std::ifstream file(entry.path());
                 std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                 if (content.find(nodeA.label) != std::string::npos) {
                     for (auto& nodeB : graph.nodes) {
                         if (nodeA.index != nodeB.index && nodeB.subjectIndex == fileToSubject[entry.path().filename().string()]) {
                             graph.addEdge(nodeA.index, nodeB.index);
                             break;
                         }
                     }
                 }
             }
        }
    }
}

void testSelfAnalysis(TestRunner& runner) {
    std::cout << "\n=== Testing Self-Analysis Graph Construction ===" << std::endl;
    Graph graph;
    constructGraphFromSource(graph);

    runner.runTest("Source graph not empty", !graph.nodes.empty(), "Should find some functions in src/");

    if (!graph.nodes.empty()) {
        std::cout << "[INFO] Found " << graph.nodes.size() << " functions as nodes." << std::endl;
        std::cout << "[INFO] Found " << graph.edgeCount() << " relationships." << std::endl;
    }
}

void testExtremeEdgeCases(TestRunner& runner) {
    std::cout << "\n=== Testing Extreme Edge Cases ===" << std::endl;

    // 1. Massive Graph Stress Test
    Graph bigGraph;
    const int numNodes = 1000;
    for (int i = 0; i < numNodes; ++i) {
        bigGraph.addNode(GraphNode("Node" + std::to_string(i), i, {}, 1, i % 10));
    }
    for (int i = 0; i < numNodes - 1; ++i) {
        bigGraph.addEdge(i, i + 1);
    }
    runner.runTest("Massive Graph Creation", bigGraph.nodes.size() == numNodes, "Should handle 1000 nodes");

    bigGraph.updateSummary();
    runner.runTest("Massive Graph Summary", bigGraph.summary.totalNodes == numNodes, "Summary should correctly count 1000 nodes");

    // 2. Deep Hierarchy Path Length
    auto pathDists = bigGraph.calculateShortestPaths(0);
    runner.runTest("Deep Path Length", pathDists[numNodes - 1] == numNodes - 1, "Should correctly calculate distance in long chain");

    // 3. Circular Dependencies
    bigGraph.addEdge(numNodes - 1, 0);
    bigGraph.updateSummary();
    runner.runTest("Circular Connectivity", bigGraph.isConnected(), "Chain with loop should be connected");

    // 4. Malformed / Extreme Indices
    bigGraph.addEdge(0, 999999);
    runner.runTest("Invalid Index Edge Prevention", bigGraph.nodeMap[0].neighbors.size() == 2, "Should not add edge to non-existent index");

    // 5. Zero Weights and Negative Subjects
    GraphNode edgeNode("EdgeNode", 2000, {}, 0, -1);
    bigGraph.addNode(edgeNode);
    runner.runTest("Negative Subject Index", bigGraph.nodeExists(2000), "Should allow negative subject index");
}

void runAll3Tests() {
    std::cout << "=== CBT Graph Editor Advanced Test Suite (Suite 3) ===" << std::endl;
    TestRunner runner;
    testSelfAnalysis(runner);
    testExtremeEdgeCases(runner);
    runner.printResults();
}
