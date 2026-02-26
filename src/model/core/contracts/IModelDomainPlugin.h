#pragma once

#include <string>

namespace brain_model::core::contracts {

// Forward declarations of registries
class GraphRegistry;
class SystemRegistry;
class ControlRegistry;
class AnalyticsRegistry;
class OverlayAnchorRegistry;

class IModelDomainPlugin {
public:
    virtual ~IModelDomainPlugin() = default;

    virtual std::string name() const = 0;

    virtual void register_entities(GraphRegistry& registry) = 0;
    virtual void register_systems(SystemRegistry& registry) = 0;
    virtual void register_controls(ControlRegistry& registry) = 0;
    virtual void register_analytics(AnalyticsRegistry& registry) = 0;
    virtual void register_overlay_anchors(OverlayAnchorRegistry& registry) = 0;
};

} // namespace brain_model::core::contracts
