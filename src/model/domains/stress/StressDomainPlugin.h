#pragma once

#include "model/core/contracts/IModelDomainPlugin.h"

namespace brain_model::domains::stress {

using namespace brain_model::core::contracts;

class StressDomainPlugin : public IModelDomainPlugin {
public:
    std::string name() const override { return "Stress"; }
    void register_entities(GraphRegistry&) override {}
    void register_systems(SystemRegistry&) override {}
    void register_controls(ControlRegistry&) override {}
    void register_analytics(AnalyticsRegistry&) override {}
    void register_overlay_anchors(OverlayAnchorRegistry&) override {}
};

} // namespace brain_model::domains::stress
