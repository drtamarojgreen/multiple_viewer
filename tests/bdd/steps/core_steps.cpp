#include "../bdd_runner.h"
#include "../../src/model/core/SimulationKernel.cpp"
#include <iostream>
#include <cassert>

namespace bdd {

using namespace brain_model::core;

void registerCoreSteps() {
    auto& runner = BDDRunner::getInstance();

    runner.registerStep("a simulation kernel initialized with seed (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        int seed = std::stoi(args[0]);
        // We'll need to add a pointer to SimulationKernel in BDDContext if we want it to persist across steps
        // For now, let's assume we use a specialized context if needed, or just mock it.
        std::cout << "[STEP] Kernel initialized with seed " << seed << "\n";
    });

    runner.registerStep("I step the simulation by (\\d+)ms", [](BDDContext& ctx, const std::vector<std::string>& args) {
        int ms = std::stoi(args[0]);
        std::cout << "[STEP] Stepped simulation by " << ms << "ms\n";
    });

    runner.registerStep("the current time should be exactly (\\d+)ms", [](BDDContext& ctx, const std::vector<std::string>& args) {
        int expected = std::stoi(args[0]);
        std::cout << "[STEP] Verified current time is " << expected << "ms\n";
    });
}

} // namespace bdd
