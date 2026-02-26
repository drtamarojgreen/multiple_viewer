#include "benchmark_runner.h"
#include <iostream>

namespace ui {

void BenchmarkRunner::runPerformanceTests(const Graph& graph) {
    std::cout << "[Benchmark] Running tests on " << graph.nodes.size() << " nodes...\n";
}

} // namespace ui
