#include "testsuite4_logic.h"
#include "../src/model/brain_model.h"
#include "../src/render/spatial_index.h"
#include "../src/model/model_repository.h"
#include <iostream>
#include <cassert>
#include <fstream>
#include <vector>

using namespace model;
using namespace render;

static int testsPassed = 0;
static int testsFailed = 0;

#define TEST_PHASE1(name, expr) do { \
    bool test_result = (expr); \
    if (test_result) { \
        ++testsPassed; \
        std::cout << "[PASS] Phase 1: " << name << "\n"; \
    } else { \
        ++testsFailed; \
        std::cout << "[FAIL] Phase 1: " << name << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
    } \
} while (0)

void testOctreeSubdivision() {
    SpatialBounds bounds{-100, -100, -100, 100, 100, 100};
    OctreeIndex index(bounds, 2); 

    index.insert(1, 10, 10, 10);
    index.insert(2, 20, 20, 20);
    index.insert(3, 30, 30, 30); // Subdivide

    auto results = index.queryRange(SpatialBounds{5, 5, 5, 15, 15, 15});
    TEST_PHASE1("Octree single point query", results.size() == 1 && results[0] == 1);
    
    auto results2 = index.queryRange(SpatialBounds{0, 0, 0, 50, 50, 50});
    TEST_PHASE1("Octree range query", results2.size() == 3);
}

void testSplineInterpolationPoints() {
    BrainPathway pathway;
    pathway.controlPoints = { {0,0,0}, {10,10,10}, {20,0,20} };
    
    auto points = pathway.getInterpolatedPoints(5);
    TEST_PHASE1("Spline points count", points.size() == 11);
    TEST_PHASE1("Spline start point", points[0].x == 0);
    TEST_PHASE1("Spline end point", points.back().x == 20);
}

void testModelVersioning() {
    BrainModel model;
    model.versionTag = "AAL3_v1";
    model.timestamp = "2026-02-26";
    TEST_PHASE1("Model version tag", model.versionTag == "AAL3_v1");
}

void testRegionHierarchyAccess() {
    BrainRegion parent;
    parent.id = "CTX";
    BrainRegion child;
    child.id = "PFC";
    child.parentID = "CTX";
    TEST_PHASE1("Region hierarchy link", child.parentID == "CTX");
}

void testSubjectMapping() {
    auto& repo = ModelRepository::getInstance();
    repo.setSubjectIndexForRegion("AMYG", 42);
    TEST_PHASE1("Region-to-Subject mapping", repo.getSubjectIndexForRegion("AMYG") == 42);
    TEST_PHASE1("Non-existent mapping", repo.getSubjectIndexForRegion("NOPE") == -1);
}

void testHotReload() {
    auto& repo = ModelRepository::getInstance();
    repo.clearAll();
    
    // Create a temporary atlas file
    std::ofstream tmp("tmp_atlas.csv");
    tmp << "REGION,R1,Region 1,0,0,0,10\n";
    tmp.close();
    
    repo.loadAtlas("tmp_atlas.csv");
    TEST_PHASE1("Initial load", repo.getModel().getRegions().size() == 1);
    
    // Modify atlas file
    std::ofstream tmp2("tmp_atlas.csv");
    tmp2 << "REGION,R1,Region 1,0,0,0,10\n";
    tmp2 << "REGION,R2,Region 2,100,100,100,5\n";
    tmp2.close();
    
    repo.reloadAtlas();
    TEST_PHASE1("Hot-Reload count", repo.getModel().getRegions().size() == 2);
    TEST_PHASE1("Hot-Reload contains R2", repo.getModel().getRegion("R2") != nullptr);
}

void testProbabilisticMembership() {
    auto& repo = ModelRepository::getInstance();
    
    // Create a temporary overlay file
    std::ofstream tmp("tmp_overlay.csv");
    tmp << "MAP,1,R1,,0.85\n"; // probabilistic mapping
    tmp.close();
    
    repo.loadOverlay("tmp_overlay.csv");
    const auto& mappings = repo.getOverlay().getAllMappings();
    bool found = false;
    for (const auto& [nodeId, m] : mappings) {
        if (m.graphNodeId == 1 && m.regionId == "R1") {
            if (std::abs(m.confidence - 0.85f) < 0.001f) found = true;
        }
    }
    TEST_PHASE1("Probabilistic confidence mapping", found);
}



void runAll4Tests() {
    std::cout << "\n=== Running Phase 1 Enhancements Test Suite ===\n";
    testOctreeSubdivision();
    testSplineInterpolationPoints();
    testModelVersioning();
    testRegionHierarchyAccess();
    testSubjectMapping();
    testHotReload();
    testProbabilisticMembership();

    
    std::cout << "Phase 1 Results: " << testsPassed << " passed, " << testsFailed << " failed.\n";
}

#ifdef STANDALONE_TEST
int main() {
    runAll4Tests();
    return 0;
}
#endif

