#include "model/core/OverlayService.h"
#include <iostream>

namespace brain_model::core {

bool OverlayService::load_overlays_from_file(const std::string& path) {
    // Mock implementation
    std::cout << "[OverlayService] Mock loading from " << path << std::endl;
    return true;
}

void OverlayService::add_overlay(const contracts::OverlaySpec& spec) {
    m_entityToOverlays[spec.anchor_entity_id].push_back(spec);
}

std::vector<contracts::OverlaySpec> OverlayService::get_active_overlays_for_entity(const std::string& entity_id) const {
    auto it = m_entityToOverlays.find(entity_id);
    if (it != m_entityToOverlays.end()) {
        return it->second;
    }
    return {};
}

void OverlayService::render_2d() {
    // No-op for now
}

void OverlayService::render_3d() {
    // No-op for now
}

} // namespace brain_model::core
