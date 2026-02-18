#ifndef TESTSUITE3_LOGIC_H
#define TESTSUITE3_LOGIC_H

#include "../src/map_logic.h"
#include "testsuite2_logic.h" // For TestRunner

void testSelfAnalysis(TestRunner& runner);
void testExtremeEdgeCases(TestRunner& runner);
void runAll3Tests();

#endif // TESTSUITE3_LOGIC_H
