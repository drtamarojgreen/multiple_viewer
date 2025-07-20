// testsuite2_logic.h
#ifndef TESTSUITE2_LOGIC_H
#define TESTSUITE2_LOGIC_H


#include "map_logic.h"
#include <set>
#include <vector>
#include <string>

// Test result tracking
struct TestResult {
    std::string name;
    bool passed;
    std::string message;
};

class TestRunner {
private:
    std::vector<TestResult> results;
    int testsPassed = 0;
    int testsFailed = 0;

public:
    void runTest(const std::string& name, bool condition, const std::string& message = "");
    void printResults();
    bool allTestsPassed() const { return testsFailed == 0; }
    void clearResults();
};

// Helper functions for parsing (missing from your codebase)
std::set<int> parseNeighborsHelper(const std::string& neighborStr);

// Test categories
void testBasicGraphOperations(TestRunner& runner);
void testFileOperations(TestRunner& runner);
void testGraphAnalytics(TestRunner& runner);
void testVisualizationFeatures(TestRunner& runner);
void testNavigationAndZoom(TestRunner& runner);
void testAdvancedFeatures(TestRunner& runner);
void testEdgeCases(TestRunner& runner);


// Main test runner
void runAll2Tests();

#endif // #define TESTSUITE2_LOGIC_H
