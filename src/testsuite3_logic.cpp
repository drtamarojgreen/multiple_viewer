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

/**
 * Heuristically constructs a graph from the project's own source code.
 * Nodes represent functions found in .cpp and .h files.
 * Subjects (groups) represent the files containing those functions.
 * Edges represent cross-file references (if a function name appears in another file).
 */
void constructGraphFromSource(Graph& graph) {
    int nodeIdx = 0;
    int subjectIdx = 0;
    std::map<std::string, int> fileToSubject;

    // Regex for basic function matching in C++
    // Matches: [ReturnType] [FunctionName]([Args]) {
    std::regex funcRegex(R"((?:void|int|bool|double|float|std::vector|std::string|GraphNode|GraphSummary)\s+([a-zA-Z0-9_:]+)\s*\([^)]*\)\s*\{)");

    // Pass 1: Identify files (subjects) and functions (nodes)
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
                        // Avoid common false positives or language keywords
                        if (funcName == "if" || funcName == "while" || funcName == "for") continue;

                        GraphNode node(funcName, nodeIdx++, {}, 1, fileToSubject[filename]);
                        graph.addNode(node);
                    }
                }
            }
        }
    }

    // Pass 2: Identify relationships (edges)
    // Heuristic: If function A's name is found in File B, create edges between A and all functions in File B.
    std::map<std::string, std::string> fileContents;
    for (const auto& entry : fs::directory_iterator("src")) {
        if (entry.is_regular_file()) {
            std::ifstream file(entry.path());
            fileContents[entry.path().filename().string()] = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        }
    }

    for (auto& [idA, nodeA] : graph.nodeMap) {
        for (auto const& [filename, content] : fileContents) {
            // If function name A is mentioned in another file
            if (content.find(nodeA.label) != std::string::npos) {
                // Connect nodeA to one representative node from that file to avoid edge explosion
                for (auto& [idB, nodeB] : graph.nodeMap) {
                    if (idA != idB && nodeB.subjectIndex == fileToSubject[filename]) {
                        graph.addEdge(idA, idB);
                        break; // Just one connection per file match for this heuristic
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

    runner.runTest("Source graph not empty", !graph.nodeMap.empty(), "Should find some functions in src/");

    if (!graph.nodeMap.empty()) {
        std::cout << "[INFO] Found " << graph.nodeMap.size() << " functions as nodes." << std::endl;
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
    runner.runTest("Massive Graph Creation", bigGraph.nodeMap.size() == numNodes, "Should handle 1000 nodes");

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
