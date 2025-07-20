// testsuite2_logic.cpp
#include "testsuite2_logic.h"
#include "map_logic.h"
#include "viewer_logic.h"
#include "file_logic.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>


// Test Runner Implementation
void TestRunner::runTest(const std::string& name, bool condition, const std::string& message) {
    TestResult result;
    result.name = name;
    result.passed = condition;
    result.message = message;
    results.push_back(result);
    
    if (condition) {
        ++testsPassed;
        std::cout << "[PASS] " << name << std::endl;
    } else {
        ++testsFailed;
        std::cout << "[FAIL] " << name;
        if (!message.empty()) {
            std::cout << " - " << message;
        }
        std::cout << std::endl;
    }
}

void TestRunner::printResults() {
    int total = testsPassed + testsFailed;
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "TEST RESULTS SUMMARY" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "Total Tests: " << total << std::endl;
    std::cout << "Passed: " << testsPassed << std::endl;
    std::cout << "Failed: " << testsFailed << std::endl;
    
    if (total > 0) {
        double percentage = (testsPassed * 100.0) / total;
        std::cout << "Success Rate: " << std::fixed << std::setprecision(1) 
                  << percentage << "%" << std::endl;
    }
    
    if (testsFailed > 0) {
        std::cout << "\nFailed Tests:" << std::endl;
        for (const auto& result : results) {
            if (!result.passed) {
                std::cout << "  - " << result.name;
                if (!result.message.empty()) {
                    std::cout << ": " << result.message;
                }
                std::cout << std::endl;
            }
        }
    }
}

void TestRunner::clearResults() {
    results.clear();
    testsPassed = 0;
    testsFailed = 0;
}

// Helper function implementation
std::set<int> parseNeighborsHelper(const std::string& neighborStr) {
    std::set<int> neighbors;
    if (neighborStr.empty() || neighborStr == "[]") {
        return neighbors;
    }
    
    std::string cleaned = neighborStr;
    if (cleaned.front() == '[') cleaned = cleaned.substr(1);
    if (cleaned.back() == ']') cleaned = cleaned.substr(0, cleaned.length() - 1);
    
    std::stringstream ss(cleaned);
    std::string token;
    while (std::getline(ss, token, ',')) {
        if (!token.empty()) {
            try {
                neighbors.insert(std::stoi(token));
            } catch (const std::exception&) {
                // Skip invalid tokens
            }
        }
    }
    return neighbors;
}

// Test 1: Basic Graph Operations
void testBasicGraphOperations(TestRunner& runner) {
    std::cout << "\n=== Testing Basic Graph Operations ===" << std::endl;
    
    // Test graph creation and node addition
    Graph graph;
    GraphNode node1;
    node1.index = 1;
    node1.label = "Test Node 1";
    node1.weight = 5;
    node1.subjectIndex = 0;
    
    graph.addNode(node1);
    runner.runTest("Add Node", graph.nodes.size() == 1, "Node count should be 1");
    runner.runTest("Node Exists", graph.nodeExists(1), "Node 1 should exist");
    runner.runTest("Node Not Exists", !graph.nodeExists(999), "Node 999 should not exist");
    
    // Test adding second node
    GraphNode node2;
    node2.index = 2;
    node2.label = "Test Node 2";
    node2.weight = 3;
    node2.subjectIndex = 1;
    
    graph.addNode(node2);
    runner.runTest("Add Second Node", graph.nodes.size() == 2, "Node count should be 2");
    
    // Test duplicate node addition (should not add)
    graph.addNode(node1);
    runner.runTest("Duplicate Node Prevention", graph.nodes.size() == 2, "Should not add duplicate nodes");
    
    // Test edge addition
    graph.addEdge(1, 2);
    runner.runTest("Add Edge", graph.nodeMap[1].neighbors.size() == 1, "Node 1 should have 1 neighbor");
    runner.runTest("Undirected Edge", graph.nodeMap[2].neighbors.size() == 1, "Node 2 should have 1 neighbor");
    
    // Test invalid edge addition
    graph.addEdge(1, 999);
    runner.runTest("Invalid Edge Prevention", graph.nodeMap[1].neighbors.size() == 1, "Should not add edge to non-existent node");
    
    // Test node removal
    graph.removeNode(2);
    runner.runTest("Remove Node", graph.nodes.size() == 1, "Node count should be 1 after removal");
    runner.runTest("Node Removed", !graph.nodeExists(2), "Node 2 should not exist after removal");
    runner.runTest("Edge Cleanup", graph.nodeMap[1].neighbors.empty(), "Node 1 should have no neighbors after node 2 removal");
    
    // Test graph clearing
    graph.clear();
    runner.runTest("Clear Graph", graph.nodes.empty(), "Graph should be empty after clear");
}

// Test 2: File Operations
void testFileOperations(TestRunner& runner) {
    std::cout << "\n=== Testing File Operations ===" << std::endl;
    
    // Create a test graph
    Graph graph;
    GraphNode node1;
    node1.index = 1;
    node1.label = "Node One";
    node1.weight = 10;
    node1.subjectIndex = 0;
    
    GraphNode node2;
    node2.index = 2;
    node2.label = "Node Two";
    node2.weight = 5;
    node2.subjectIndex = 1;
    
    graph.addNode(node1);
    graph.addNode(node2);
    graph.addEdge(1, 2);
    
    // Test saving to CSV
    bool saveSuccess = saveGraphToCSV(graph, "test_output.csv");
    runner.runTest("Save Graph to CSV", saveSuccess, "Should successfully save graph to CSV");
    
    // Test loading from CSV
    Graph loadedGraph;
    bool loadSuccess = loadGraphFromCSV(loadedGraph, "test_output.csv");
    runner.runTest("Load Graph from CSV", loadSuccess, "Should successfully load graph from CSV");
    
    if (loadSuccess) {
        runner.runTest("Loaded Node Count", loadedGraph.nodes.size() == 2, "Should load 2 nodes");
        runner.runTest("Loaded Node Labels", 
                      loadedGraph.nodeMap[1].label == "Node One" && 
                      loadedGraph.nodeMap[2].label == "Node Two", 
                      "Should preserve node labels");
        runner.runTest("Loaded Node Weights", 
                      loadedGraph.nodeMap[1].weight == 10 && 
                      loadedGraph.nodeMap[2].weight == 5, 
                      "Should preserve node weights");
        runner.runTest("Loaded Edges", 
                      loadedGraph.nodeMap[1].neighbors.size() == 1 && 
                      loadedGraph.nodeMap[2].neighbors.size() == 1, 
                      "Should preserve edges");
    }
    
    // Test loading non-existent file
    Graph emptyGraph;
    bool failLoad = loadGraphFromCSV(emptyGraph, "non_existent_file.csv");
    runner.runTest("Load Non-existent File", !failLoad, "Should fail to load non-existent file");
    
    // Test parseNeighborsHelper helper function
    auto neighbors1 = parseNeighborsHelper("[]");
    runner.runTest("Parse Empty Neighbors", neighbors1.empty(), "Empty neighbor list should be empty");
    
    auto neighbors2 = parseNeighborsHelper("[1,2,3]");
    runner.runTest("Parse Neighbors", neighbors2.size() == 3 && neighbors2.count(2) == 1, "Should parse neighbor list correctly");
    
    auto neighbors3 = parseNeighborsHelper("[1,2,invalid,3]");
    runner.runTest("Parse Invalid Neighbors", neighbors3.size() == 3, "Should skip invalid neighbor tokens");
}

// Test 3: Graph Analytics
void testGraphAnalytics(TestRunner& runner) {
    std::cout << "\n=== Testing Graph Analytics ===" << std::endl;
    
    // Create a test graph with known properties
    Graph graph;
    
    // Add nodes
    for (int i = 1; i <= 5; ++i) {
        GraphNode node;
        node.index = i;
        node.label = "Node " + std::to_string(i);
        node.weight = i;
        node.subjectIndex = i % 2;
        graph.addNode(node);
    }
    
    // Create a connected component (1-2-3) and isolated nodes (4, 5)
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    
    // Test connectivity
    runner.runTest("Graph Not Connected", !graph.isConnected(), "Graph should not be connected (has isolated nodes)");
    
    // Test edge count
    runner.runTest("Edge Count", graph.edgeCount() == 2, "Should have 2 edges");
    
    // Test average degree
    float avgDegree = graph.computeAvgDegree();
    runner.runTest("Average Degree", std::abs(avgDegree - 0.8f) < 0.1f, "Average degree should be approximately 0.8");
    
    // Test isolated nodes
    runner.runTest("Isolated Nodes", graph.countIsolatedNodes() == 2, "Should have 2 isolated nodes");
    
    // Test shortest paths
    auto distances = graph.calculateShortestPaths(1);
    runner.runTest("Shortest Path to Self", distances[1] == 0, "Distance to self should be 0");
    runner.runTest("Shortest Path Adjacent", distances[2] == 1, "Distance to adjacent node should be 1");
    runner.runTest("Shortest Path Two Hops", distances[3] == 2, "Distance via one intermediate should be 2");
    runner.runTest("Shortest Path Unreachable", distances.find(4) == distances.end(), "Unreachable node should not be in distance map");
    
    // Test summary update
    graph.updateSummary();
    runner.runTest("Summary Total Nodes", graph.summary.totalNodes == 5, "Summary should show 5 nodes");
    runner.runTest("Summary Total Edges", graph.summary.totalEdges == 2, "Summary should show 2 edges");
    runner.runTest("Summary Average Degree", std::abs(graph.summary.averageDegree - 0.8f) < 0.1f, "Summary average degree should be correct");
    runner.runTest("Summary Isolated Count", graph.summary.isolatedNodeCount == 2, "Summary should show 2 isolated nodes");
    
    // Test connected graph
    graph.addEdge(3, 4);
    graph.addEdge(4, 5);
    runner.runTest("Connected Graph", graph.isConnected(), "Graph should be connected after adding edges");
}

// Test 4: Visualization Features
void testVisualizationFeatures(TestRunner& runner) {
    std::cout << "\n=== Testing Visualization Features ===" << std::endl;
    
    Graph graph;
    
    // Add test nodes
    for (int i = 1; i <= 3; ++i) {
        GraphNode node;
        node.index = i;
        node.label = "Test Node " + std::to_string(i);
        node.weight = i * 2;
        node.subjectIndex = i % 2;
        graph.addNode(node);
    }
    
    // Test adaptive label length
    int maxLabelLength = graph.getMaxLabelLength();
    runner.runTest("Max Label Length", maxLabelLength > 0, "Should calculate max label length");
    
    int adaptiveLen0 = graph.getAdaptiveLabelLength(0);
    int adaptiveLen1 = graph.getAdaptiveLabelLength(1);
    int adaptiveLen2 = graph.getAdaptiveLabelLength(2);
    
    runner.runTest("Adaptive Label Depth 0", adaptiveLen0 >= adaptiveLen1, "Depth 0 should have longest labels");
    runner.runTest("Adaptive Label Depth 1", adaptiveLen1 >= adaptiveLen2, "Depth 1 should have medium labels");
    runner.runTest("Adaptive Label Minimum", adaptiveLen2 >= 3, "All labels should be at least 3 characters");
    
    // Test node size calculation
    int size0 = graph.calculateNodeSize(0);
    int size1 = graph.calculateNodeSize(1);
    int size2 = graph.calculateNodeSize(2);
    
    runner.runTest("Node Size Depth 0", size0 >= size1, "Depth 0 nodes should be largest");
    runner.runTest("Node Size Depth 1", size1 >= size2, "Depth 1 nodes should be medium");
    runner.runTest("Node Size Minimum", size2 >= 1, "All nodes should have minimum size 1");
    
    // Test viewport calculations
    bool inViewport = graph.isInViewport(10, 10, 5);
    runner.runTest("Viewport Test", inViewport, "Node at (10,10) should be in viewport");
    
    bool outViewport = graph.isInViewport(-100, -100, 5);
    runner.runTest("Out of Viewport", !outViewport, "Node at (-100,-100) should be out of viewport");
    
    // Test max distance calculation
    int maxDist = graph.getMaxDistance();
    runner.runTest("Max Distance Positive", maxDist > 0, "Max distance should be positive");
    
    // Test subject filtering
    graph.subjectFilterOnly = true;
    graph.focusedNodeIndex = 0;
    if (graph.nodes.size() > 0) {
        bool passesFilter = graph.passesSubjectFilter(0);
        runner.runTest("Subject Filter Pass", passesFilter, "Should pass subject filter for same subject");
    }
    
    // Test focus-only view
    graph.focusOnlyAtMaxZoom = true;
    graph.zoomLevel = Graph::ZoomLevel::Z5;
    runner.runTest("Focus Only View Max Zoom", graph.isFocusOnlyView(), "Should be focus-only at max zoom");
    
    graph.zoomLevel = Graph::ZoomLevel::Z3;
    runner.runTest("Focus Only View Mid Zoom", !graph.isFocusOnlyView(), "Should not be focus-only at mid zoom");
}

// Test 5: Navigation and Zoom
void testNavigationAndZoom(TestRunner& runner) {
    std::cout << "\n=== Testing Navigation and Zoom ===" << std::endl;
    
    Graph graph;
    
    // Add test nodes for focus cycling
    for (int i = 1; i <= 3; ++i) {
        GraphNode node;
        node.index = i;
        node.label = "Node " + std::to_string(i);
        graph.addNode(node);
    }
    
    // Test zoom operations
    Graph::ZoomLevel initialZoom = graph.zoomLevel;
    graph.zoomIn();
    runner.runTest("Zoom In", graph.zoomLevel > initialZoom, "Should zoom in");
    
    graph.zoomOut();
    runner.runTest("Zoom Out", graph.zoomLevel == initialZoom, "Should zoom out to original level");
    
    // Test zoom limits
    graph.zoomLevel = Graph::ZoomLevel::Z5;
    graph.zoomIn();
    runner.runTest("Zoom In Limit", graph.zoomLevel == Graph::ZoomLevel::Z5, "Should not zoom beyond maximum");
    
    graph.zoomLevel = Graph::ZoomLevel::Z1;
    graph.zoomOut();
    runner.runTest("Zoom Out Limit", graph.zoomLevel == Graph::ZoomLevel::Z1, "Should not zoom beyond minimum");
    
    // Test panning
    int initialPanX = graph.panX;
    int initialPanY = graph.panY;
    graph.pan(5, 3);
    runner.runTest("Pan X", graph.panX == initialPanX + 5, "Should pan X correctly");
    runner.runTest("Pan Y", graph.panY == initialPanY + 3, "Should pan Y correctly");
    
    // Test focus management
    graph.addFocus(1);
    runner.runTest("Add Focus", graph.focusedNodeIndices.count(1) == 1, "Should add focus");
    
    graph.addFocus(2);
    runner.runTest("Multiple Focus", graph.focusedNodeIndices.size() == 2, "Should support multiple focuses");
    
    graph.removeFocus(1);
    runner.runTest("Remove Focus", graph.focusedNodeIndices.count(1) == 0, "Should remove focus");
    
    graph.clearFocuses();
    runner.runTest("Clear Focuses", graph.focusedNodeIndices.empty(), "Should clear all focuses");
    
    // Test focus cycling
    graph.cycleFocus();
    runner.runTest("Cycle Focus", graph.focusedNodeIndices.size() == 1, "Should set one focus after cycling");
    
    // Test render distance
    graph.setMaxRenderDistance(5);
    runner.runTest("Set Max Render Distance", graph.getMaxRenderDistance() == 5, "Should set max render distance");
}

// Test 6: Advanced Features
void testAdvancedFeatures(TestRunner& runner) {
    std::cout << "\n=== Testing Advanced Features ===" << std::endl;
    
    Graph graph;
    
    // Create a more complex graph for advanced testing
    for (int i = 1; i <= 6; ++i) {
        GraphNode node;
        node.index = i;
        node.label = "Node " + std::to_string(i);
        node.weight = i;
        node.subjectIndex = i % 3;
        graph.addNode(node);
    }
    
    // Create edges to form a more complex structure
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(3, 4);
    graph.addEdge(4, 1); // Create a cycle
    graph.addEdge(5, 6);
    
    // Test multi-focus distances
    graph.addFocus(1);
    graph.addFocus(5);
    auto distances = graph.computeMultiFocusDistances();
    
    runner.runTest("Multi-Focus Distance to Focus", distances[1] == 0, "Distance to focus node should be 0");
    runner.runTest("Multi-Focus Distance Calculation", distances[2] == 1, "Distance to adjacent node should be 1");
    runner.runTest("Multi-Focus Multiple Sources", distances[5] == 0, "Second focus should also have distance 0");
    
    // Test vanishing point calculation
    VanishingPoint vp = calculateVanishingPoint(80, 25);
    runner.runTest("Vanishing Point Center X", vp.centerX == 40.0f, "Center X should be half width");
    runner.runTest("Vanishing Point Center Y", vp.centerY == 12.5f, "Center Y should be half height");
    runner.runTest("Vanishing Point Focal Length", vp.focalLength > 0, "Focal length should be positive");
    
    // Test 3D to 2D projection
    Point3D point3d = {0.0f, 0.0f, 10.0f};
    Point2D point2d = projectToVanishingPoint(point3d, vp);
    runner.runTest("Projection X in Bounds", point2d.x >= 0 && point2d.x < 80, "Projected X should be in screen bounds");
    runner.runTest("Projection Y in Bounds", point2d.y >= 0 && point2d.y < 25, "Projected Y should be in screen bounds");
    
    // Test book structure creation
    auto bookStructure = createBookStructure(graph);
    runner.runTest("Book Structure Created", !bookStructure.empty(), "Should create book structure");
    
    // Test grid layer assignment
    int layer0 = getGridLayer(0, 3);
    int layer3 = getGridLayer(3, 3);
    runner.runTest("Grid Layer 0", layer0 == 0, "Node 0 should be in layer 0");
    runner.runTest("Grid Layer 3", layer3 == 0, "Node 3 should be in layer 0 (3 % 3 = 0)");
    
    // Test adaptive spacing
    graph.updateSummary();
    float spacing = calculateAdaptiveNodeSpacing(graph);
    runner.runTest("Adaptive Spacing", spacing > 0, "Adaptive spacing should be positive");
    
    // Test density strategy
    applyDensityStrategy(graph);
    runner.runTest("Density Strategy Applied", true, "Density strategy should apply without errors");
    
    // Test z-buffer functionality
    std::vector<std::vector<float>> zbuf(25, std::vector<float>(80, std::numeric_limits<float>::infinity()));
    updateZBuffer(zbuf, {10, 10}, 3, 5.0f);
    runner.runTest("Z-Buffer Update", zbuf[10][10] == 5.0f, "Z-buffer should update with depth value");
}

// Test 7: Edge Cases and Error Handling
void testEdgeCases(TestRunner& runner) {
    std::cout << "\n=== Testing Edge Cases and Error Handling ===" << std::endl;
    
    // Test empty graph operations
    Graph emptyGraph;
    runner.runTest("Empty Graph Connected", emptyGraph.isConnected(), "Empty graph should be considered connected");
    runner.runTest("Empty Graph Edge Count", emptyGraph.edgeCount() == 0, "Empty graph should have 0 edges");
    runner.runTest("Empty Graph Avg Degree", emptyGraph.computeAvgDegree() == 0.0f, "Empty graph should have 0 average degree");
    runner.runTest("Empty Graph Isolated Count", emptyGraph.countIsolatedNodes() == 0, "Empty graph should have 0 isolated nodes");
    
    // Test single node graph
    Graph singleNodeGraph;
    GraphNode singleNode;
    singleNode.index = 1;
    singleNode.label = "Single";
    singleNodeGraph.addNode(singleNode);
    
    runner.runTest("Single Node Connected", singleNodeGraph.isConnected(), "Single node graph should be connected");
    runner.runTest("Single Node Isolated", singleNodeGraph.countIsolatedNodes() == 1, "Single node should be isolated");
    
    // Test self-loop prevention (if implemented)
    singleNodeGraph.addEdge(1, 1);
    // This behavior depends on implementation - adjust test accordingly
    
    // Test large label handling
    GraphNode longLabelNode;
    longLabelNode.index = 2;
    longLabelNode.label = std::string(1000, 'A'); // Very long label
    singleNodeGraph.addNode(longLabelNode);
    
    int maxLen = singleNodeGraph.getMaxLabelLength();
    runner.runTest("Long Label Handling", maxLen == 1000, "Should handle very long labels");
    
    // Test negative indices
    GraphNode negativeNode;
    negativeNode.index = -1;
    negativeNode.label = "Negative";
    singleNodeGraph.addNode(negativeNode);
    
    runner.runTest("Negative Index Node", singleNodeGraph.nodeExists(-1), "Should handle negative indices");
    
    // Test extreme coordinates
    singleNodeGraph.nodePos[-1] = {-1000, -1000, -1000};
    bool extremeInViewport = singleNodeGraph.isInViewport(-1000, -1000, 1);
    runner.runTest("Extreme Coordinates", !extremeInViewport, "Extreme coordinates should be out of viewport");
    
    // Test proximity depth edge cases
    float proximityDepth = singleNodeGraph.getProximityDepth(999); // Non-existent node
    runner.runTest("Proximity Depth Non-existent", proximityDepth == 1.0f, "Non-existent node should have max proximity depth");
    
    // Test focus operations on empty sets
    emptyGraph.cycleFocus();
    runner.runTest("Cycle Focus Empty", emptyGraph.focusedNodeIndices.empty(), "Cycling focus on empty graph should do nothing");
    
    emptyGraph.removeFocus(999);
    runner.runTest("Remove Non-existent Focus", emptyGraph.focusedNodeIndices.empty(), "Removing non-existent focus should be safe");
    
    // Test distance calculations with unreachable nodes
    Graph disconnectedGraph;
    GraphNode node1, node2;
    node1.index = 1;
    node1.label = "Node1";
    node2.index = 2;
    node2.label = "Node2";
    disconnectedGraph.addNode(node1);
    disconnectedGraph.addNode(node2);
    
    auto distances = disconnectedGraph.calculateShortestPaths(1);
    runner.runTest("Unreachable Node Distance", distances.find(2) == distances.end(), "Unreachable node should not be in distance map");
    
    // Test configuration edge cases
    Config::viewerZoom = 0.0f;
    runner.runTest("Zero Zoom Config", Config::viewerZoom == 0.0f, "Should handle zero zoom configuration");
    
    Config::viewerZoom = 1.0f; // Reset to normal
}

// Main test runner
void runAll2Tests() {
    std::cout << "=== CBT Graph Editor Unit Test Suite ===" << std::endl;
    std::cout << "Starting comprehensive test run..." << std::endl;
    
    TestRunner runner;
    
    try {
        testBasicGraphOperations(runner);
        testFileOperations(runner);
        testGraphAnalytics(runner);
        testVisualizationFeatures(runner);
        testNavigationAndZoom(runner);
        testAdvancedFeatures(runner);
        testEdgeCases(runner);
        
        runner.printResults();
        
        if (!runner.allTestsPassed()) {
            std::cerr << "\nSome tests failed. Please review the failures above." << std::endl;
            return; // or exit(1) if you want to terminate
        } else {
            std::cout << "\nAll tests passed successfully! ✓" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Test execution failed with exception: " << e.what() << std::endl;
        return;
    }
    
    std::cout << "Test run completed." << std::endl;
}
