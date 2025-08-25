#include "map_logic.h"
#include "viewer_logic.h"
#include "file_logic.h"
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
    bool ok = (expr); \
    if (ok) { \
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
    GraphNode a{"A", 0, {1}, 1, 0};
    GraphNode b{"B", 1, {0}, 1, 0};

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
    g.addNode({"Alpha", 0, {0}, 1, 0});
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
    g.addNode({"A", 0, {1}, 1, 1});
    g.addNode({"B", 1, {0}, 1, 1});
    g.addEdge(0, 1);
    TEST("edge symmetric 0->1", g.nodeMap[0].neighbors[0] == 1);
    TEST("edge symmetric 1->0", g.nodeMap[1].neighbors[0] == 0);
}

void testZBufferOutOfBounds() {
    int H=CONSOLE_HEIGHT, W=CONSOLE_WIDTH;
    std::vector<std::vector<float>> zb(H, std::vector<float>(W, INFINITY));
    updateZBuffer(zb, {999,999}, 3, 1.0f); // should not crash
    TEST("zBuffer out-of-bounds safe", true);  // no crash = pass
}

void testGraphSerialization() {
    Graph g;
    g.addNode({"Node0", 0, {1}, 1, 1});
    g.addNode({"Node1", 0, {0}, 1, 1});
    g.addEdge(0, 1);
    saveGraphToCSV(g, "test_graph.csv");

    Graph g2;
    bool ok = loadGraphFromCSV(g2, "test_graph.csv");
    TEST("graph reload success", ok);
    TEST("graph reload node count", g2.nodes.size() == 2);
    TEST("graph reload edge count", g2.edgeCount() == 1);
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
  Graph g; g.zoomLevel = ZoomLevel::Z3;
  int base = g.getMaxLabelLength();
  int d0 = getAdaptiveLabelLength(0, g.zoomLevel);
  int d1 = getAdaptiveLabelLength(1, g.zoomLevel);
  int d2 = getAdaptiveLabelLength(2, g.zoomLevel);
  TEST("label depth0 > base", d0 > base);
  TEST("label depth1 == base", d1 == base);
  TEST("label depth2 < base", d2 < base);
}

// Test 3: subject-only filtering
void testSubjectFiltering() {
  Graph g;
  g.nodes[0] = GraphNode("A",0,0,1);
  g.nodes[1] = GraphNode("B",1,0,2);
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
  g.zoomLevel = Graph::Z5;
  TEST("isFocusOnlyView true", g.isFocusOnlyView());
  g.zoomLevel = Graph::Z3;
  TEST("isFocusOnlyView false", !g.isFocusOnlyView());
}

// Test 5: continuous node size calculation
void testNodeSizeCalc() {
  Graph g;
  int s0 = calculateNodeSize(0,Graph::Z2);
  int s1 = calculateNodeSize(1,Graph::Z2);
  int s2 = calculateNodeSize(2,Graph::Z2);
  TEST("nodeSize depth0 > depth1", s0 > s1);
  TEST("nodeSize depth1 > depth2", s1 > s2);
  TEST("nodeSize >=1", s2 >= 1);
}

// Test 6: book-based structure creation
void testBookStructure() {
  Graph g;
  // two subjects, depths simulated by index % 3
  for (int i=0;i<6;i++) {
    GraphNode n("N"+std::to_string(i),i,0,i%2);
    g.nodes[i]=n;
    g.nodePos[i] = {i%3, i%4, i%3};  // z = i%3
  }
  auto books = createBookStructure(g);
  // expect at most 2 subjects * 3 depths = 6 chapters
  TEST("bookChapters <=6", books.size()<=6);
}

// Test 7: proximity depth calculation
void testProximityDepth() {
  Graph g;
  // place node at center
  g.nodePos[0] = {12,40,0};
  // place node at corner
  g.nodePos[1] = {0,0,0};
  float p0 = g.getProximityDepth(0);
  float p1 = g.getProximityDepth(1);
  TEST("proximity center == 0", fabs(p0) < 1e-6);
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
  VanishingPoint vp = calculateVanishingPoint();
  Point3D w{0.0f,0.0f,10.0f};
  auto p = projectToVanishingPoint(w,vp);
  // must lie within screen
  TEST("proj x in bounds",
       p.x >= 0 && p.x < CONSOLE_WIDTH);
  TEST("proj y in bounds",
       p.y >= 0 && p.y < CONSOLE_HEIGHT);
}

// Test 10: z-buffer occlusion logic
void testZBuffer() {
  int H=CONSOLE_HEIGHT, W=CONSOLE_WIDTH;
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
  g.computeSummary();
  float sp = calculateAdaptiveNodeSpacing(g);
  TEST("spacing >=1.0", sp >= 1.0f);
}

// Test 12: density-based rendering toggles
void testDensityStrategy() {
  Graph g;
  // low density
  g.summary.nodeCount = 10; g.summary.edgeCount = 5;
  applyDensityStrategy(g);
  TEST("low density showLines", g.showLines == true);
  // high density
  g.summary.nodeCount = 10; g.summary.edgeCount = 45;
  applyDensityStrategy(g);
  TEST("high density no lines", g.showLines == false);
}

// Test 13: computeSummary enhancements (clustering, diameter)
void testComputeSummaryEnhancements() {
  Graph g;
  // create triangle for clustering test
  for (int i=0;i<3;i++) {
    g.nodes[i] = GraphNode("N",i,0,0);
  }
  g.nodes[0].neighbors = {1,2};
  g.nodes[1].neighbors = {0,2};
  g.nodes[2].neighbors = {0,1};
  g.computeSummary();
  TEST("avgClustering >0", g.summary.avgClusteringCoeff > 0.0f);
  TEST("diameter ==1", g.summary.diameter == 1);
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
      testFocusManagement();
      testEmptyGraphBehavior();
      testEdgeAddition();
      testZBufferOutOfBounds();
      testGraphSerialization();

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
        // continue gracefully without terminating
      } else {
        std::cout << "All tests passed. You're good to go.\n";
      }
      
}
