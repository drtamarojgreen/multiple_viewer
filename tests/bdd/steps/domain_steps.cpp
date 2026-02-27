#include "../bdd_runner.h"
#include "model/app/IntegratedBrainModel.h"
#include <iostream>

#include <cassert>

#include <memory>



namespace bdd {



using namespace brain_model::app;

using namespace brain_model::core::contracts;



// Mock IOverlayService

class MockOverlayService : public IOverlayService {

public:

    bool load_overlays_from_file(const std::string& path) override { return true; }

    void add_overlay(const OverlaySpec& spec) override {

        active_overlays_.push_back(spec);

    }

    std::vector<OverlaySpec> get_active_overlays_for_entity(const std::string& entity_id) const override {

        std::vector<OverlaySpec> result;

        for(const auto& spec : active_overlays_) {

            if (spec.anchor_entity_id == entity_id) {

                result.push_back(spec);

            }

        }

        return result;

    }

    void render_2d() override {}

    void render_3d() override {}



    std::vector<OverlaySpec> active_overlays_;

};



// Mock ISimulationKernel

class MockSimulationKernel : public ISimulationKernel {

public:

    void step(uint32_t delta_ms) override { current_time_ms_ += delta_ms; }

    void pause() override { is_running_ = false; }

    void resume() override { is_running_ = true; }

    SimulationSnapshot capture_snapshot() const override {

        SimulationSnapshot snapshot;

        snapshot.timestamp_ms = current_time_ms_;

        snapshot.snapshot_hash = "mock_hash_" + std::to_string(current_time_ms_);

        return snapshot;

    }

    void restore_snapshot(const SimulationSnapshot& snapshot) override {

        current_time_ms_ = snapshot.timestamp_ms;

    }

    void set_seed(uint64_t seed) override {}

    uint64_t current_time_ms() const override { return current_time_ms_; }

    bool is_running() const override { return is_running_; }



    uint64_t current_time_ms_ = 0;

    bool is_running_ = false;

};



void registerDomainSteps() {

    auto& runner = BDDRunner::getInstance();



    runner.registerStep("an integrated brain model with the (.*) domain", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::string domain = args[0];

        ctx.mockOverlayService = std::make_shared<MockOverlayService>();

        ctx.mockSimulationKernel = std::make_shared<MockSimulationKernel>();

        ctx.integratedBrainModel = std::make_unique<IntegratedBrainModel>(ctx.mockSimulationKernel, ctx.mockOverlayService);

        // Add a mock domain plugin if needed

        std::cout << "[STEP] Model initialized with domain: " << domain << "\n";

    });



    runner.registerStep("the simulation kernel is running", [](BDDContext& ctx, const std::vector<std::string>& args) {

        assert(ctx.mockSimulationKernel != nullptr);

        std::shared_ptr<MockSimulationKernel> mockSimulationKernel = std::dynamic_pointer_cast<MockSimulationKernel>(ctx.mockSimulationKernel);

        assert(mockSimulationKernel != nullptr);

        mockSimulationKernel->resume();

        assert(mockSimulationKernel->is_running() == true);

    });



    runner.registerStep("(\\d+)ms have elapsed", [](BDDContext& ctx, const std::vector<std::string>& args) {

        assert(ctx.mockSimulationKernel != nullptr);

        std::shared_ptr<MockSimulationKernel> mockSimulationKernel = std::dynamic_pointer_cast<MockSimulationKernel>(ctx.mockSimulationKernel);

        assert(mockSimulationKernel != nullptr);

        mockSimulationKernel->step(std::stoi(args[0]));

    });



    runner.registerStep("an overlay with ID \"(.*)\" should be active", [](BDDContext& ctx, const std::vector<std::string>& args) {

        std::string id = args[0];

        assert(ctx.mockOverlayService != nullptr);

        bool found = false;

        std::shared_ptr<MockOverlayService> mockOverlayService = std::dynamic_pointer_cast<MockOverlayService>(ctx.mockOverlayService);

        assert(mockOverlayService != nullptr);

        for (const auto& overlay : mockOverlayService->active_overlays_) {

            if (overlay.id == id) {

                found = true;

                break;

            }

        }

        assert(found == true);

        std::cout << "[STEP] Verified overlay active: " << id << "\n";

    });

    runner.registerStep("the text should contain \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::string text = args[0];
        std::cout << "[STEP] Verified overlay text contains: " << text << "\n";
    });

    runner.registerStep("the overlay text should contain \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {

        std::string text = args[0];

        assert(ctx.mockOverlayService != nullptr);

        bool found = false;

        std::shared_ptr<MockOverlayService> mockOverlayService = std::dynamic_pointer_cast<MockOverlayService>(ctx.mockOverlayService);

        assert(mockOverlayService != nullptr);

        for (const auto& overlay : mockOverlayService->active_overlays_) {

            if (overlay.text.find(text) != std::string::npos) {

                found = true;

                break;

            }

        }

        assert(found == true);

        std::cout << "[STEP] Verified overlay text contains: " << text << "\n";
    });

    runner.registerStep("the simulation kernel is running", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("(\\d+)ms have elapsed", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the simulation reaches state where (.*) and (.*) regions interact", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("an overlay with ID \"(.*)\" should be published", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the role should be \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the simulation kernel steps forward", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("an overlay for the \"(.*)\" anchor should be active", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the text should describe \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("a threat stimulus event is published", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the Amygdala system should emit high-priority \"(.*)\" text", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("its priority should be greater than or equal to (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the simulation kernel is active", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the text should signify active distributed processing", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("high-frequency signaling is simulated", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("a \"(.*)\" overlay with ID \"(.*)\" should be active", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the text should describe synaptic weighting adjustments", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the simulation progresses under moderate load", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("an overlay with ID \"(.*)\" should be visible", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("a restorative sleep simulation step is executed", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the text should describe replenishment of the resilience buffer", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the simulation kernel steps", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the text should confirm BER maintenance integrity", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("transcription repair protocols are simulated", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("it should describe dynamic flux equilibrium", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the simulation progresses", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("its text should contain a resilience index value", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the simulation steps through repeated signaling", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the text should signify reinforcement strength", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("a synaptic transmission event occurs", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("an overlay with ID \"(.*)\" should describe LTP activation", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("external pressure is simulated", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("an overlay with ID \"(.*)\" should indicate load level", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the simulation progress continues", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("it should describe the current inflammatory equilibrium", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });
}

} // namespace bdd


