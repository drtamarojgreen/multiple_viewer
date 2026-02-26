#ifndef BENCHMARK_RUNNER_H
#define BENCHMARK_RUNNER_H

#include "../map_logic.h"

namespace ui {

class BenchmarkRunner {
public:
    static void runPerformanceTests(const Graph& graph);
};

} // namespace ui

#endif // BENCHMARK_RUNNER_H
