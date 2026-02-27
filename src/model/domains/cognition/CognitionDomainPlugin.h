#pragma once

#include "model/core/contracts/IModelDomainPlugin.h"

namespace brain_model::domains::cognition {

using namespace brain_model::core::contracts;

class CognitionDomainPlugin : public IModelDomainPlugin {
public:
    std::string name() const override { return "Cognition"; }

    void register_entities(GraphRegistry& registry) override {
        // Cognition-specific entity registration
    }

    void register_systems(SystemRegistry& registry) override {
        // Cognition-specific simulation system registration
    }

    void register_controls(ControlRegistry& registry) override {
        // Cognition-specific user control registration
    }

    void register_analytics(AnalyticsRegistry& registry) override {
        // Cognition-specific analytics emitter registration
    }

    void register_overlay_anchors(OverlayAnchorRegistry& registry) override {
        // Cognition-specific overlay anchor registration
    }
};

} // namespace brain_model::domains::cognition
