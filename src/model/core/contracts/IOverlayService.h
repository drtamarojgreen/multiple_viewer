#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>

namespace brain_model::core::contracts {

struct OverlaySpec {
    std::string id;
    std::string domain;
    std::string anchor_entity_id;
    int priority;
    std::string text;
    std::string role;
};

class IOverlayService {
public:
    virtual ~IOverlayService() = default;

    // Spec Management
    virtual bool load_overlays_from_file(const std::string& path) = 0;
    virtual void add_overlay(const OverlaySpec& spec) = 0;
    
    // Binding & Resolution
    virtual std::vector<OverlaySpec> get_active_overlays_for_entity(const std::string& entity_id) const = 0;
    
    // Rendering Support
    virtual void render_2d() = 0;
    virtual void render_3d() = 0;
};

} // namespace brain_model::core::contracts
