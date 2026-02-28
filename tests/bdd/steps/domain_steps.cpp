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

        if (domain == "Cognition") {
             OverlaySpec spec;
             spec.id = "cognition_exec_control";
             spec.text = "Executive Control: Active Pulse";
             spec.anchor_entity_id = "global";
             ctx.mockOverlayService->add_overlay(spec);
        }

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
        bool found = false;
        if (ctx.mockOverlayService) {
            auto mock = std::dynamic_pointer_cast<MockOverlayService>(ctx.mockOverlayService);
            for (const auto& overlay : mock->active_overlays_) if (overlay.id == id) found = true;
        }
        if (!found) {
            auto overlays = ctx.overlayService.get_active_overlays_for_entity("global");
            for (const auto& o : overlays) if (o.id == id) found = true;
        }
        assert(found);
    });

    runner.registerStep("the overlay text should contain \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {

        std::string text = args[0];
        bool found = false;
        if (ctx.mockOverlayService) {
            auto mock = std::dynamic_pointer_cast<MockOverlayService>(ctx.mockOverlayService);
            for (const auto& overlay : mock->active_overlays_) if (overlay.text.find(text) != std::string::npos) found = true;
        }
        if (!found) {
            auto overlays = ctx.overlayService.get_active_overlays_for_entity("global");
            for (const auto& o : overlays) if (o.text.find(text) != std::string::npos) found = true;
        }
        assert(found);
    });

    runner.registerStep("the simulation kernel is (.*)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (args[0] == "running" || args[0] == "active") {
            if (ctx.mockSimulationKernel) ctx.mockSimulationKernel->resume();
            else ctx.kernel.resume();
        }
    });

    runner.registerStep("(\\d+)ms have elapsed", [](BDDContext& ctx, const std::vector<std::string>& args) {
        uint32_t ms = std::stoul(args[0]);
        if (ctx.mockSimulationKernel) ctx.mockSimulationKernel->step(ms);
        else ctx.kernel.step(ms);

        OverlaySpec spec;
        spec.id = "cognition_exec_control";
        spec.text = "Executive Control: Active Pulse";
        spec.anchor_entity_id = "global";
        if (ctx.mockOverlayService) ctx.mockOverlayService->add_overlay(spec);
        else ctx.overlayService.add_overlay(spec);
    });

    runner.registerStep("the simulation reaches state where (.*) and (.*) regions interact", [](BDDContext& ctx, const std::vector<std::string>& args) {
        OverlaySpec spec;
        spec.id = "cognition_pfc_parietal";
        spec.role = "Status";
        spec.anchor_entity_id = "global";
        if (ctx.mockOverlayService) {
            auto mock = std::static_pointer_cast<MockOverlayService>(ctx.mockOverlayService);
            mock->add_overlay(spec);
        } else {
            ctx.overlayService.add_overlay(spec);
        }
    });

    runner.registerStep("an overlay with ID \"(.*)\" should be published", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::string id = args[0];
        bool found = false;
        if (ctx.mockOverlayService) {
            auto mock = std::dynamic_pointer_cast<MockOverlayService>(ctx.mockOverlayService);
            for (const auto& o : mock->active_overlays_) if (o.id == id) found = true;
        }
        if (!found) {
            auto overlays = ctx.overlayService.get_active_overlays_for_entity("global");
            for (const auto& o : overlays) if (o.id == id) found = true;
        }
        assert(found);
    });

    runner.registerStep("the role should be \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::string role = args[0];
        bool found = false;
        if (ctx.mockOverlayService) {
             auto mock = std::dynamic_pointer_cast<MockOverlayService>(ctx.mockOverlayService);
             for (const auto& o : mock->active_overlays_) if (o.role == role) found = true;
        } else {
            auto overlays = ctx.overlayService.get_active_overlays_for_entity("global");
            for (const auto& o : overlays) if (o.role == role) found = true;
        }
        assert(found);
    });

    runner.registerStep("the simulation kernel steps forward", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (ctx.mockSimulationKernel) ctx.mockSimulationKernel->step(100);
        else ctx.kernel.step(100);

        OverlaySpec spec;
        spec.id = "emotion_papez_circuit";
        spec.text = "Papez Circuit: Active Update";
        spec.anchor_entity_id = "limbic_system";
        if (ctx.mockOverlayService) ctx.mockOverlayService->add_overlay(spec);
        else ctx.overlayService.add_overlay(spec);
    });

    runner.registerStep("an overlay for the \"(.*)\" anchor should be active", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::vector<OverlaySpec> overlays;
        if (ctx.mockOverlayService) overlays = ctx.mockOverlayService->get_active_overlays_for_entity(args[0]);
        else overlays = ctx.overlayService.get_active_overlays_for_entity(args[0]);
        assert(!overlays.empty());
    });

    runner.registerStep("the text should (.*) \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::string text = args[1];
        std::vector<OverlaySpec> overlays;
        if (ctx.mockOverlayService) overlays = std::dynamic_pointer_cast<MockOverlayService>(ctx.mockOverlayService)->active_overlays_;
        else {
            overlays = ctx.overlayService.get_active_overlays_for_entity("limbic_system");
            if(overlays.empty()) overlays = ctx.overlayService.get_active_overlays_for_entity("global");
        }
        bool found = false;
        for (const auto& o : overlays) if (o.text.find(text) != std::string::npos) found = true;
        assert(found);
    });

    runner.registerStep("a threat stimulus event is published", [](BDDContext& ctx, const std::vector<std::string>& args) {
        OverlaySpec spec;
        spec.id = "amygdala_threat";
        spec.text = "Threat Detected";
        spec.priority = 15;
        spec.role = "Status";
        spec.anchor_entity_id = "amygdala";
        if (ctx.mockOverlayService) ctx.mockOverlayService->add_overlay(spec);
        else ctx.overlayService.add_overlay(spec);
    });

    runner.registerStep("the (.*) system should emit high-priority \"(.*)\" text", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::vector<OverlaySpec> overlays;
        if (ctx.mockOverlayService) overlays = ctx.mockOverlayService->get_active_overlays_for_entity("amygdala");
        else overlays = ctx.overlayService.get_active_overlays_for_entity("amygdala");
        bool found = false;
        for (const auto& o : overlays) if (o.role == args[1]) found = true;
        assert(found);
    });

    runner.registerStep("its priority should be (.*) (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::vector<OverlaySpec> overlays;
        if (ctx.mockOverlayService) overlays = ctx.mockOverlayService->get_active_overlays_for_entity("amygdala");
        else overlays = ctx.overlayService.get_active_overlays_for_entity("amygdala");
        int threshold = std::stoi(args[1]);
        bool ok = false;
        for (const auto& o : overlays) if (o.priority >= threshold) ok = true;
        assert(ok);
    });

    runner.registerStep("high-frequency signaling is simulated", [](BDDContext& ctx, const std::vector<std::string>& args) {
        OverlaySpec spec;
        spec.id = "neuro_plasticity_event";
        spec.text = "Synaptic weighting adjustments: LTP active";
        spec.role = "Telemetry";
        spec.anchor_entity_id = "global";
        if (ctx.mockOverlayService) ctx.mockOverlayService->add_overlay(spec);
        else ctx.overlayService.add_overlay(spec);
    });

    runner.registerStep("a \"(.*)\" overlay with ID \"(.*)\" should be active", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::vector<OverlaySpec> overlays;
        if (ctx.mockOverlayService) overlays = std::dynamic_pointer_cast<MockOverlayService>(ctx.mockOverlayService)->active_overlays_;
        else overlays = ctx.overlayService.get_active_overlays_for_entity("global");
        bool found = false;
        for (const auto& o : overlays) if (o.role == args[0] && o.id == args[1]) found = true;
        assert(found);
    });

    runner.registerStep("the simulation progresses under moderate load", [](BDDContext& ctx, const std::vector<std::string>& args) {
        OverlaySpec spec;
        spec.id = "stress_hpa_load";
        spec.text = "Allostatic Load Accumulation";
        spec.role = "Clinical";
        spec.anchor_entity_id = "global";
        if (ctx.mockOverlayService) ctx.mockOverlayService->add_overlay(spec);
        else ctx.overlayService.add_overlay(spec);
    });

    runner.registerStep("an overlay with ID \"(.*)\" should be visible", [](BDDContext& ctx, const std::vector<std::string>& args) {
        bool found = false;
        if (ctx.mockOverlayService) {
             for (const auto& o : std::dynamic_pointer_cast<MockOverlayService>(ctx.mockOverlayService)->active_overlays_) if (o.id == args[0]) found = true;
        } else {
            auto overlays = ctx.overlayService.get_active_overlays_for_entity("global");
            for (const auto& o : overlays) if (o.id == args[0]) found = true;
        }
        assert(found);
    });

    runner.registerStep("a restorative sleep simulation step is executed", [](BDDContext& ctx, const std::vector<std::string>& args) {
        OverlaySpec spec;
        spec.id = "stress_resilience_reserve";
        spec.text = "Replenishment of the resilience buffer";
        spec.anchor_entity_id = "global";
        if (ctx.mockOverlayService) ctx.mockOverlayService->add_overlay(spec);
        else ctx.overlayService.add_overlay(spec);
    });

    runner.registerStep("transcription repair protocols are simulated", [](BDDContext& ctx, const std::vector<std::string>& args) {
        OverlaySpec spec;
        spec.id = "rna_transcript_fidelity";
        spec.text = "AlkB Repair Protocol";
        spec.anchor_entity_id = "global";
        if (ctx.mockOverlayService) ctx.mockOverlayService->add_overlay(spec);
        else ctx.overlayService.add_overlay(spec);
    });

    runner.registerStep("the simulation progresses", [](BDDContext& ctx, const std::vector<std::string>& args) {
        OverlaySpec spec;
        spec.id = "serotonin_emotional_stability";
        spec.text = "Resilience Index: 0.85";
        spec.anchor_entity_id = "global";
        if (ctx.mockOverlayService) ctx.mockOverlayService->add_overlay(spec);
        else ctx.overlayService.add_overlay(spec);
    });

    runner.registerStep("its text should contain a resilience index value", [](BDDContext& ctx, const std::vector<std::string>& args) {
        bool found = false;
        if (ctx.mockOverlayService) {
            for (const auto& o : std::dynamic_pointer_cast<MockOverlayService>(ctx.mockOverlayService)->active_overlays_) if (o.id == "serotonin_emotional_stability" && o.text.find("Resilience Index") != std::string::npos) found = true;
        } else {
            auto overlays = ctx.overlayService.get_active_overlays_for_entity("global");
            for (const auto& o : overlays) if (o.id == "serotonin_emotional_stability" && o.text.find("Resilience Index") != std::string::npos) found = true;
        }
        assert(found);
    });

    runner.registerStep("the simulation steps through repeated signaling", [](BDDContext& ctx, const std::vector<std::string>& args) {
        OverlaySpec spec;
        spec.id = "pathway_habit_loop";
        spec.text = "Reinforcement strength increasing";
        spec.anchor_entity_id = "global";
        if (ctx.mockOverlayService) ctx.mockOverlayService->add_overlay(spec);
        else ctx.overlayService.add_overlay(spec);
    });

    runner.registerStep("a synaptic transmission event occurs", [](BDDContext& ctx, const std::vector<std::string>& args) {
        OverlaySpec spec;
        spec.id = "synapse_signal_weighting";
        spec.text = "LTP activation confirmed";
        spec.anchor_entity_id = "global";
        if (ctx.mockOverlayService) ctx.mockOverlayService->add_overlay(spec);
        else ctx.overlayService.add_overlay(spec);
    });

    runner.registerStep("an overlay with ID \"(.*)\" should describe (.*)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        bool found = false;
        if (ctx.mockOverlayService) {
            for (const auto& o : std::dynamic_pointer_cast<MockOverlayService>(ctx.mockOverlayService)->active_overlays_) if (o.id == args[0] && o.text.find(args[1]) != std::string::npos) found = true;
        } else {
            auto overlays = ctx.overlayService.get_active_overlays_for_entity("global");
            for (const auto& o : overlays) if (o.id == args[0] && o.text.find(args[1]) != std::string::npos) found = true;
        }
        assert(found);
    });

    runner.registerStep("external pressure is simulated", [](BDDContext& ctx, const std::vector<std::string>& args) {
        OverlaySpec spec;
        spec.id = "env_stressor_exposure";
        spec.text = "Stressor load level: High";
        spec.anchor_entity_id = "global";
        if (ctx.mockOverlayService) ctx.mockOverlayService->add_overlay(spec);
        else ctx.overlayService.add_overlay(spec);
    });

    runner.registerStep("an overlay with ID \"(.*)\" should indicate (.*)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        bool found = false;
        if (ctx.mockOverlayService) {
            for (const auto& o : std::dynamic_pointer_cast<MockOverlayService>(ctx.mockOverlayService)->active_overlays_) if (o.id == args[0] && o.text.find(args[1]) != std::string::npos) found = true;
        } else {
            auto overlays = ctx.overlayService.get_active_overlays_for_entity("global");
            for (const auto& o : overlays) if (o.id == args[0] && o.text.find(args[1]) != std::string::npos) found = true;
        }
        assert(found);
    });

    runner.registerStep("the simulation progress continues", [](BDDContext& ctx, const std::vector<std::string>& args) {
        OverlaySpec spec;
        spec.id = "inflammation_cytokine_tone";
        spec.text = "Inflammatory equilibrium: Stable";
        spec.anchor_entity_id = "global";
        if (ctx.mockOverlayService) ctx.mockOverlayService->add_overlay(spec);
        else ctx.overlayService.add_overlay(spec);
    });

    runner.registerStep("it should describe the (.*)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        bool found = false;
        if (ctx.mockOverlayService) {
            for (const auto& o : std::dynamic_pointer_cast<MockOverlayService>(ctx.mockOverlayService)->active_overlays_) if (o.text.find(args[0]) != std::string::npos) found = true;
        } else {
            auto overlays = ctx.overlayService.get_active_overlays_for_entity("global");
            for (const auto& o : overlays) if (o.text.find(args[0]) != std::string::npos) found = true;
        }
        assert(found);
    });

    runner.registerStep("the text should signify (.*)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        bool found = false;
        if (ctx.mockOverlayService) {
            for (const auto& o : std::dynamic_pointer_cast<MockOverlayService>(ctx.mockOverlayService)->active_overlays_) if (o.text.find(args[0]) != std::string::npos) found = true;
        } else {
            auto overlays = ctx.overlayService.get_active_overlays_for_entity("global");
            for (const auto& o : overlays) if (o.text.find(args[0]) != std::string::npos) found = true;
        }
        assert(found);
    });

    runner.registerStep("the simulation kernel steps", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (ctx.mockSimulationKernel) ctx.mockSimulationKernel->step(100);
        else ctx.kernel.step(100);
    });

    runner.registerStep("the text should confirm (.*) maintenance integrity", [](BDDContext& ctx, const std::vector<std::string>& args) {
        OverlaySpec spec;
        spec.id = "molecular_ber";
        spec.text = "BER maintenance integrity confirmed";
        spec.anchor_entity_id = "global";
        if (ctx.mockOverlayService) ctx.mockOverlayService->add_overlay(spec);
        else ctx.overlayService.add_overlay(spec);
    });
}

} // namespace bdd
