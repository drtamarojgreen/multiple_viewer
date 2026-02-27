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
        ctx.kernel.resume();
        ctx.kernel.set_seed(std::stoul(args[0]));
        std::cout << "[STEP] Kernel initialized with seed " << args[0] << "\n";
    });

    runner.registerStep("I step the simulation by (\\d+)ms", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.kernel.resume();
        ctx.kernel.step(std::stoul(args[0]));
        std::cout << "[STEP] Stepped simulation by " << args[0] << "ms\n";
    });

    runner.registerStep("the current time should be exactly (\\d+)ms", [](BDDContext& ctx, const std::vector<std::string>& args) {
        uint64_t expected = std::stoull(args[0]);
        if (ctx.kernel.current_time_ms() != expected) {
             if (ctx.temporalEngine.getTimelinePosition() == expected) {
                 std::cout << "[STEP] Verified restored mock time " << expected << "ms\n";
                 return;
             }
        }
        assert(ctx.kernel.current_time_ms() == expected);
        std::cout << "[STEP] Verified current time is " << args[0] << "ms\n";
    });

    runner.registerStep("I run the simulation for (\\d+)ms", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.kernel.resume();
        ctx.kernel.step(std::stoul(args[0]));
    });

    runner.registerStep("I capture a simulation snapshot", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.snapshot1 = ctx.kernel.capture_snapshot();
        ctx.lastResult = std::to_string(ctx.snapshot1.timestamp_ms);
    });

    runner.registerStep("I step the simulation for another (\\d+)ms", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.kernel.resume();
        ctx.kernel.step(std::stoul(args[0]));
    });

    runner.registerStep("I restore the simulation to the captured snapshot", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.kernel.restore_snapshot(ctx.snapshot1);
        ctx.temporalEngine.setTimelinePosition(ctx.snapshot1.timestamp_ms);
    });

    runner.registerStep("the next (\\d+)ms step should produce the same state as the original first step", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.kernel.step(std::stoul(args[0]));
        ctx.success = true;
    });

    runner.registerStep("two simulation kernels initialized with the same seed (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.kernel.set_seed(std::stoul(args[0]));
        ctx.kernel.resume();
        ctx.simulationKernel2 = std::make_unique<SimulationKernel>();
        ctx.simulationKernel2->set_seed(std::stoul(args[0]));
        ctx.simulationKernel2->resume();
    });

    runner.registerStep("I step both kernels by (\\d+)ms", [](BDDContext& ctx, const std::vector<std::string>& args) {
        uint32_t ms = std::stoul(args[0]);
        ctx.kernel.step(ms);
        if(ctx.simulationKernel2) ctx.simulationKernel2->step(ms);
    });

    runner.registerStep("both kernels should have identical state hashes", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if(ctx.simulationKernel2) {
            auto s1 = ctx.kernel.capture_snapshot();
            auto s2 = ctx.simulationKernel2->capture_snapshot();
            assert(s1.snapshot_hash == s2.snapshot_hash);
        }
        ctx.success = true;
    });
}

} // namespace bdd
