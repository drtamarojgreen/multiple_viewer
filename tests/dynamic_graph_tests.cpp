#include "dynamic_graph_tests.h"
#include "../src/map_logic.h"
#include "../src/analysis_logic.h"
#include "testsuite2_logic.h"
#include <iostream>
#include <vector>
#include <string>

void testVariableSizes(TestRunner& runner) {
    std::cout << "\n=== Testing Variable Graph Sizes ===" << std::endl;

    std::vector<int> sizes = {0, 10, 100, 1000, 5000};
    for (int size : sizes) {
        Graph g;
        for (int i = 0; i < size; ++i) {
            g.addNode(GraphNode("Node" + std::to_string(i), i));
        }
        runner.runTest("Size " + std::to_string(size) + " creation", (int)g.nodeMap.size() == size);

        g.updateSummary();
        runner.runTest("Size " + std::to_string(size) + " summary update", g.summary.totalNodes == size);
    }
}

void testTopologies(TestRunner& runner) {
    std::cout << "\n=== Testing Graph Topologies ===" << std::endl;

    // 1. Star Topology
    {
        Graph star;
        int center = 0;
        star.addNode(GraphNode("Center", center));
        for (int i = 1; i <= 10; ++i) {
            star.addNode(GraphNode("Leaf" + std::to_string(i), i));
            star.addEdge(center, i);
        }
        star.updateSummary();
        runner.runTest("Star Topology: Degree of center", star.nodeMap.at(center).neighbors.size() == 10);
        runner.runTest("Star Topology: Edge count", star.summary.totalEdges == 10);
        runner.runTest("Star Topology: Connected", star.summary.isConnected);
    }

    // 2. Clique (Complete Graph)
    {
        Graph clique;
        int n = 10;
        for (int i = 0; i < n; ++i) clique.addNode(GraphNode("N", i));
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                clique.addEdge(i, j);
            }
        }
        clique.updateSummary();
        int expectedEdges = n * (n - 1) / 2;
        runner.runTest("Clique Topology: Edge count", clique.summary.totalEdges == expectedEdges);
        runner.runTest("Clique Topology: Density", clique.summary.density > 0.99f); // Density 1.0
    }

    // 3. Grid Topology (Simple 2x2)
    {
        Graph grid;
        // 0-1
        // | |
        // 2-3
        for (int i = 0; i < 4; ++i) grid.addNode(GraphNode("G", i));
        grid.addEdge(0, 1);
        grid.addEdge(0, 2);
        grid.addEdge(1, 3);
        grid.addEdge(2, 3);
        grid.updateSummary();
        runner.runTest("Grid Topology: Edge count", grid.summary.totalEdges == 4);
        runner.runTest("Grid Topology: Diameter", grid.summary.diameter == 2);
    }
}

void testSpecificEdgeCases(TestRunner& runner) {
    std::cout << "\n=== Testing Specific Edge Cases ===" << std::endl;

    // 1. Self-loops
    {
        Graph g;
        g.addNode(GraphNode("Self", 0));
        g.addEdge(0, 0);
        // Depending on implementation, self-loops might be ignored or counted.
        // Let's check how many neighbors.
        runner.runTest("Self-loop handling", g.nodeMap.at(0).neighbors.size() <= 1);
    }

    // 2. Multiple edges between same pair
    {
        Graph g;
        g.addNode(GraphNode("A", 0));
        g.addNode(GraphNode("B", 1));
        g.addEdge(0, 1);
        g.addEdge(0, 1);
        runner.runTest("Multi-edge prevention", g.nodeMap.at(0).neighbors.size() == 1);
    }

    // 3. Disconnected Components
    {
        Graph g;
        g.addNode(GraphNode("A1", 0));
        g.addNode(GraphNode("A2", 1));
        g.addEdge(0, 1);
        g.addNode(GraphNode("B1", 2));
        g.addNode(GraphNode("B2", 3));
        g.addEdge(2, 3);
        g.updateSummary();
        runner.runTest("Disconnected Components: Connected check", !g.summary.isConnected);
        runner.runTest("Disconnected Components: Component count", (int)g.summary.components.size() == 2);
    }

    // 4. Extreme Weights and Labels
    {
        Graph g;
        std::string longLabel(1000, 'X');
        g.addNode(GraphNode(longLabel, 0, {}, 999999, 1));
        runner.runTest("Long label support", g.nodeMap.at(0).label.length() == 1000);
        runner.runTest("High weight support", g.nodeMap.at(0).weight == 999999);
    }
}

void runDynamicGraphTests() {
    std::cout << "\n=== CBT Graph Editor Dynamic Size & Edge Case Suite ===" << std::endl;
    TestRunner runner;
    testVariableSizes(runner);
    testTopologies(runner);
    testSpecificEdgeCases(runner);
    runner.printResults();
}
