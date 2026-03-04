#include "../bdd_runner.h"
#include "model/core/SimulationKernel.h"
#include <iostream>
#include <cassert>
#include <cstring>

namespace bdd {

using namespace brain_model::core;

void registerCoreSteps() {
    auto& runner = BDDRunner::getInstance();

    runner.registerStep("a simulation kernel initialized with seed (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.simulationKernel = std::make_unique<SimulationKernel>();
        ctx.simulationKernel->resume();
        ctx.simulationKernel->set_seed(std::stoul(args[0]));
        std::cout << "[STEP] Kernel initialized with seed " << args[0] << "\n";
    });

    runner.registerStep("I step the simulation by (\\d+)ms", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.simulationKernel != nullptr);
        ctx.simulationKernel->step(std::stoul(args[0]));
        std::cout << "[STEP] Stepped simulation by " << args[0] << "ms\n";
    });

    runner.registerStep("the current time should be exactly (\\d+)ms", [](BDDContext& ctx, const std::vector<std::string>& args) {
        uint64_t expected = std::stoull(args[0]);
        assert(ctx.simulationKernel != nullptr);
        if (ctx.simulationKernel->current_time_ms() != expected) {
             std::cerr << "[BDD FAIL] Expected time " << expected << "ms, got " << ctx.simulationKernel->current_time_ms() << "ms" << std::endl;
        }
        assert(ctx.simulationKernel->current_time_ms() == expected);
        std::cout << "[STEP] Verified current time is " << args[0] << "ms\n";
    });

    runner.registerStep("I run the simulation for (\\d+)ms", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.simulationKernel != nullptr);
        ctx.simulationKernel->step(std::stoul(args[0]));
    });

    runner.registerStep("I capture a simulation snapshot", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.simulationKernel != nullptr);
        ctx.snapshot1 = ctx.simulationKernel->capture_snapshot();
        ctx.lastResult = std::to_string(ctx.snapshot1.timestamp_ms);
    });

    runner.registerStep("I step the simulation for another (\\d+)ms", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.simulationKernel != nullptr);
        ctx.simulationKernel->step(std::stoul(args[0]));
    });

    runner.registerStep("I restore the simulation to the captured snapshot", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.simulationKernel != nullptr);
        ctx.simulationKernel->restore_snapshot(ctx.snapshot1);
    });

    runner.registerStep("the next (\\d+)ms step should produce the same state as the original first step", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.simulationKernel != nullptr);
        uint64_t beforeTime = ctx.simulationKernel->current_time_ms();
        ctx.simulationKernel->step(std::stoul(args[0]));
        assert(ctx.simulationKernel->current_time_ms() == beforeTime + std::stoul(args[0]));
    });

    runner.registerStep("two simulation kernels initialized with the same seed (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        uint64_t seed = std::stoul(args[0]);
        ctx.simulationKernel = std::make_unique<SimulationKernel>();
        ctx.simulationKernel->set_seed(seed);
        ctx.simulationKernel->resume();

        ctx.simulationKernel2 = std::make_unique<SimulationKernel>();
        ctx.simulationKernel2->set_seed(seed);
        ctx.simulationKernel2->resume();
    });

    runner.registerStep("I step both kernels by (\\d+)ms", [](BDDContext& ctx, const std::vector<std::string>& args) {
        uint32_t ms = std::stoul(args[0]);
        assert(ctx.simulationKernel != nullptr);
        assert(ctx.simulationKernel2 != nullptr);
        ctx.simulationKernel->step(ms);
        ctx.simulationKernel2->step(ms);
        ctx.snapshot1 = ctx.simulationKernel->capture_snapshot();
        ctx.snapshot2 = ctx.simulationKernel2->capture_snapshot();
    });

    runner.registerStep("both kernels should have identical state hashes", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(!ctx.snapshot1.snapshot_hash.empty());
        assert(ctx.snapshot1.snapshot_hash == ctx.snapshot2.snapshot_hash);
    });
}

} // namespace bdd
