#pragma once

#include "core/contracts/IModelDomainPlugin.h"

namespace brain_model::domains::serotonin {

using namespace brain_model::core::contracts;

class SerotoninDomainPlugin : public IModelDomainPlugin {
public:
    std::string name() const override { return "Serotonin"; }
    void register_entities(GraphRegistry&) override {}
    void register_systems(SystemRegistry&) override {}
    void register_controls(ControlRegistry&) override {}
    void register_analytics(AnalyticsRegistry&) override {}
    void register_overlay_anchors(OverlayAnchorRegistry&) override {}
};

} // namespace brain_model::domains::serotonin
