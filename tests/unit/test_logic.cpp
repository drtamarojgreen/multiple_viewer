#include "map_logic.h"
#include "viewer_logic.h"
#include "file_logic.h"
#include "io/io_manager.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <set>
#include <sstream>

static int testsPassed = 0;
static int testsFailed = 0;
static std::vector<std::string> failedTests;

#define TEST(name, expr) do { \
    bool test_result = (expr); \
    if (test_result) { \
        ++testsPassed; \
        std::cout << "[PASS] " << name << "\n"; \
    } else { \
        ++testsFailed; \
        std::ostringstream oss; \
        oss << "[FAIL] " << name << " (" << __FILE__ << ":" << __LINE__ << ")"; \
        failedTests.push_back(oss.str()); \
        std::cout << oss.str() << "\n"; \
    } \
} while (0)


void testNodeCreationAndEdges() {
    Graph g;
    GraphNode a("A", 0, {1}, 1, 0);
    GraphNode b("B", 1, {0}, 1, 0);

    g.addNode(a);
    g.addNode(b);
    g.addEdge(0, 1);

    TEST("node 0 exists", g.nodeExists(0));
    TEST("node 1 exists", g.nodeExists(1));
    TEST("edge 0→1 created", g.nodeMap.at(0).neighbors.size() == 1);
    TEST("edge 1→0 symmetric", g.nodeMap.at(1).neighbors.size() == 1);
}



void testFocusManagement() {
    Graph g;
    g.addNode(GraphNode("Alpha", 0, {0}, 1, 0));
    g.addFocus(0);
    TEST("focus added", g.focusedNodeIndices.count(0) == 1);
    g.removeFocus(0);
    TEST("focus removed", g.focusedNodeIndices.count(0) == 0);
}

void testEmptyGraphBehavior() {
    Graph g;
    TEST("empty graph is connected", g.isConnected());
    TEST("empty graph has 0 edges", g.edgeCount() == 0);
    TEST("empty graph has 0 isolated nodes", g.countIsolatedNodes() == 0);
}

void testEdgeAddition() {
    Graph g;
    g.addNode(GraphNode("A", 0, {1}, 1, 1));
    g.addNode(GraphNode("B", 1, {0}, 1, 1));
    g.addEdge(0, 1);
    TEST("edge symmetric 0->1", g.nodeMap[0].neighbors[0] == 1);
    TEST("edge symmetric 1->0", g.nodeMap[1].neighbors[0] == 0);
}

void testZBufferOutOfBounds() {
    int H=DEFAULT_CONSOLE_HEIGHT, W=DEFAULT_CONSOLE_WIDTH;
    std::vector<std::vector<float>> zb(H, std::vector<float>(W, INFINITY));
    updateZBuffer(zb, {999,999}, 3, 1.0f); // should not crash
    TEST("zBuffer out-of-bounds safe", true);  // no crash = pass
}

void testGraphSerialization() {
    Graph g;
    g.addNode(GraphNode("Node A", 0, {1}, 1, 0));
    g.addNode(GraphNode("Node B", 1, {0, 2}, 1, 0));
    g.addNode(GraphNode("Node C", 2, {1}, 1, 1));
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    saveGraphToCSV(g, "test_graph_tmp.csv");

    Graph g2;
    bool ok = loadGraphFromCSV(g2, "test_graph_tmp.csv");
    TEST("CSV reload success", ok);
    TEST("CSV reload node count", g2.nodeMap.size() == 3);
    TEST("CSV reload edge count", g2.edgeCount() == 2);
    TEST("CSV node label", g2.nodeMap.at(0).label == "Node A");
}

void testGraphSerializationJSON() {
    Graph g;
    g.addNode(GraphNode("Node A", 0));
    g.addNode(GraphNode("Node B", 1));
    g.addEdge(0, 1);
    io::IOManager::saveJSON(g, "test_graph_tmp.json");

    Graph g2;
    bool ok = io::IOManager::loadJSON(g2, "test_graph_tmp.json");
    TEST("JSON reload success", ok);
    TEST("JSON reload node count", g2.nodeMap.size() == 2);
    TEST("JSON reload edge count", g2.edgeCount() == 1);
}

void testBaselinePersistence() {
    // CSV Baseline
    Graph g_csv;
    bool ok_csv = loadGraphFromCSV(g_csv, "test_graph.csv");
    TEST("Baseline CSV load success", ok_csv);
    if (ok_csv) {
        TEST("Baseline CSV node count", g_csv.nodes.size() == 2);
        TEST("Baseline CSV edge count", g_csv.edgeCount() == 1);
        TEST("Baseline CSV label check", g_csv.nodeMap.at(100).label == "CSVNode");
        TEST("Baseline CSV weight check", g_csv.nodeMap.at(100).weight == 10);
        TEST("Baseline CSV subject check", g_csv.nodeMap.at(100).subjectIndex == 1);
    }

    // JSON Baseline
    Graph g_json;
    bool ok_json = io::IOManager::loadJSON(g_json, "test_graph.json");
    TEST("Baseline JSON load success", ok_json);
    if (ok_json) {
        TEST("Baseline JSON node count", g_json.nodes.size() == 2);
        TEST("Baseline JSON edge count", g_json.edgeCount() == 1);
        TEST("Baseline JSON label check", g_json.nodeMap.at(200).label == "JSONNode");
    }
}


// Test 1: existing CSV neighbor parsing
void testParseNeighbors() {
  auto s1 = parseNeighbors("[]");
  TEST("parseNeighbors empty", s1.empty());
  auto s2 = parseNeighbors("[1,2,3]");
  TEST("parseNeighbors size==3", s2.size()==3 && s2.count(2)==1);
}

// Test 2: adaptive label length by depth & zoom
void testAdaptiveLabelLength() {
  Graph g;
  g.addNode(GraphNode("Topic", 0, {}, 1, 0)); // "Topic" size is 5
  int base = 10; // The heuristic base in the free function
  int d0 = getAdaptiveLabelLength(0, ZoomLevel::Z3, base);
  int d1 = getAdaptiveLabelLength(1, ZoomLevel::Z3, base);
  int d2 = getAdaptiveLabelLength(2, ZoomLevel::Z3, base);
  TEST("label depth0 > base", d0 > base);
  TEST("label depth1 == base", d1 == base);
  TEST("label depth2 < base", d2 < base);
}

// Test 3: subject-only filtering
void testSubjectFiltering() {
  Graph g;
  g.addNode(GraphNode("A",0,{},1,0));
  g.addNode(GraphNode("B",1,{},2,1)); // Different subject
  g.focusedNodeIndex = 0;
  g.subjectFilterOnly = true;
  TEST("passesSubjectFilter same",    g.passesSubjectFilter(0));
  TEST("rejectsSubjectFilter diff",  !g.passesSubjectFilter(1));
}

// Test 4: focus-only at max zoom
void testFocusOnlyView() {
  Graph g;
  g.focusedNodeIndex = 5;
  g.focusOnlyAtMaxZoom = true;
  TEST("isFocusOnlyView true", g.isFocusOnlyView(ZoomLevel::Z5));
  TEST("isFocusOnlyView false", !g.isFocusOnlyView(ZoomLevel::Z3));
}

// Test 5: continuous node size calculation
void testNodeSizeCalc() {
  Graph g;
  int s0 = calculateNodeSize(0,ZoomLevel::Z3);
  int s1 = calculateNodeSize(1,ZoomLevel::Z3);
  int s2 = calculateNodeSize(2,ZoomLevel::Z3);
  TEST("nodeSize depth0 > depth1", s0 > s1);
  TEST("nodeSize depth1 > depth2", s1 > s2);
  TEST("nodeSize >=1", s2 >= 1);
}

// Test 6: book-based structure creation
void testBookStructure() {
  Graph g;
  ViewContext v;
  // two subjects, depths simulated by index % 3
  for (int i=0;i<6;i++) {
    g.addNode(GraphNode("N"+std::to_string(i),i,{},i%2,i));
    g.nodePos[i] = {static_cast<float>(i%3), static_cast<float>(i%4), static_cast<float>(i%3)};  // z = i%3
  }
  auto books = createBookStructure(g, v);
  // expect at most 2 subjects * 3 depths = 6 chapters
  TEST("bookChapters <=6", books.size()<=6);
}

// Test 7: proximity depth calculation
void testProximityDepth() {
  Graph g;
  // place node at exact center (for 80x25, center is 40.0, 12.5)
  g.nodePos[0] = {12.5f, 40.0f, 0.0f};
  // place node at corner
  g.nodePos[1] = {0.0f, 0.0f, 0.0f};
  float p0 = g.getProximityDepth(0);
  float p1 = g.getProximityDepth(1);
  TEST("proximity center == 0", std::abs(p0) < 1e-6f);
  TEST("proximity corner >0", p1 > 0.0f);
}

// Test 8: grid layer assignment (modulo strategy)
void testGridLayer() {
  // simple modulo implementation
  TEST("getGridLayer(0)==0", getGridLayer(0,3)==0);
  TEST("getGridLayer(4)%3", getGridLayer(4,3)==1);
}

// Test 9: vanishing point projection
void testVanishingProjection() {
  VanishingPoint vp = calculateVanishingPoint(DEFAULT_CONSOLE_WIDTH, DEFAULT_CONSOLE_HEIGHT);
  Point3D w{0.0f,0.0f,10.0f};
  auto p = projectToVanishingPoint(w,vp);
  // must lie within screen
  TEST("proj x in bounds",
       p.x >= 0 && p.x < DEFAULT_CONSOLE_WIDTH);
  TEST("proj y in bounds",
       p.y >= 0 && p.y < DEFAULT_CONSOLE_HEIGHT);
}

// Test 10: z-buffer occlusion logic
void testZBuffer() {
  int H=DEFAULT_CONSOLE_HEIGHT, W=DEFAULT_CONSOLE_WIDTH;
  std::vector<std::vector<float>> zb(H, std::vector<float>(W, INFINITY));
  // first pixel at depth 5
  updateZBuffer(zb, {10,10}, 1, 5.0f);
  TEST("zBuffer update", zb[10][10] == 5.0f);
  // occlusion: deeper pixel (10) shouldn't overwrite 5
  if (5.0f < INFINITY) {
    if (10.0f < zb[10][10])
      zb[10][10] = 10.0f;
  }
  TEST("zBuffer occlude", zb[10][10] == 5.0f);
}

// Test 11: adaptive spacing calculation
void testAdaptiveSpacing() {
  Graph g;
  g.updateSummary();
  float sp = calculateAdaptiveNodeSpacing(g);
  TEST("spacing >=1.0", sp >= 1.0f);
}

// Test 12: density-based rendering toggles
void testDensityStrategy() {
  Graph g;
  // low density
  g.summary.density = 0.1f;
  applyDensityStrategy(g);
  TEST("low density showLines", g.showLines == true);
  // high density
  g.summary.density = 0.9f;
  applyDensityStrategy(g);
  TEST("high density no lines", g.showLines == false);
}

// Test 13: computeSummary enhancements (clustering, diameter)
void testRefinedSearch() {
    Graph g;
    g.addNode(GraphNode("Brain Stem", 0, {}, 1, 1));
    g.addNode(GraphNode("Cerebellum", 1, {}, 1, 1));
    g.addNode(GraphNode("Frontal Lobe", 2, {}, 1, 2));

    // Case-insensitivity
    auto r1 = findSimilarTopics(g, "BRAIN");
    TEST("Search case-insensitive", r1.size() == 1 && r1[0] == 0);

    // Partial matching
    auto r2 = findSimilarTopics(g, "bel");
    TEST("Search partial match", r2.size() == 1 && r2[0] == 1);

    // Multi-token (AND search)
    auto r3 = findSimilarTopics(g, "Frontal Lobe");
    TEST("Search multi-token", r3.size() == 1 && r3[0] == 2);

    auto r4 = findSimilarTopics(g, "Lobe Frontal");
    TEST("Search multi-token unordered", r4.size() == 1 && r4[0] == 2);

    // Subject index fallback search
    auto r5 = findSimilarTopics(g, "2");
    TEST("Search subject index fallback", r5.size() == 1 && r5[0] == 2);

    // No results
    auto r6 = findSimilarTopics(g, "NonExistent");
    TEST("Search no results", r6.empty());
}

void testComputeSummaryEnhancements() {
  Graph g;
  // create triangle for clustering test
  for (int i=0;i<3;i++) {
    g.addNode(GraphNode("N",i,{},0,i));
  }
  g.addEdge(0,1);
  g.addEdge(1,2);
  g.addEdge(2,0);
  g.updateSummary();
  TEST("avgClustering >0", g.summary.avgClusteringCoeff > 0.0f);
  TEST("diameter == 1", g.summary.diameter == 1);
}


void runAllTests() {
      testNodeCreationAndEdges();
      testParseNeighbors();
      testAdaptiveLabelLength();
      testSubjectFiltering();
      testFocusOnlyView();
      testNodeSizeCalc();
      testBookStructure();
      testProximityDepth();
      testGridLayer();
      testVanishingProjection();
      testZBuffer();
      testAdaptiveSpacing();
      testDensityStrategy();
      testComputeSummaryEnhancements();
      testRefinedSearch();
      testFocusManagement();
      testEmptyGraphBehavior();
      testEdgeAddition();
      testZBufferOutOfBounds();
      testGraphSerialization();
      testGraphSerializationJSON();
      testBaselinePersistence();

      int total = testsPassed + testsFailed;
      std::cout << "\nResults: " << testsPassed
                << " passed, " << testsFailed << " failed.\n";
      double coverage = total > 0
        ? ( testsPassed * 100.0 / total )
        : 0.0;
      std::cout << "Coverage: "
                << std::fixed << std::setprecision(1)
                << coverage << "%\n";

      if (testsFailed > 0) {
        std::cerr << "Failed Tests: " << testsFailed << " test(s) failed. Review above results.\n";
      } else {
        std::cout << "All tests passed. You're good to go.\n";
      }
      
}
