#include "test_logic.h"
#include "testsuite2_logic.h"
#include "testsuite3_logic.h"
#include "testsuite4_logic.h"
#include "dynamic_graph_tests.h"
#include <iostream>

void testQuantaGliaManagement();

int main() {
    std::cout << "=== Running Unit Tests ===\n";
    runAllTests();
    runAll2Tests();
    runAll3Tests();
    runAll4Tests();
    testQuantaGliaManagement();
    runDynamicGraphTests();
    std::cout << "=== Unit Tests Completed ===\n";
    return 0;
}
