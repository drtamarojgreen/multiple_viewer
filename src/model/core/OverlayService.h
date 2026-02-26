#pragma once

#include "model/core/contracts/IOverlayService.h"
#include <unordered_map>
#include <vector>
#include <string>

namespace brain_model::core {

class OverlayService : public contracts::IOverlayService {
public:
    bool load_overlays_from_file(const std::string& path) override;
    void add_overlay(const contracts::OverlaySpec& spec) override;

    std::vector<contracts::OverlaySpec> get_active_overlays_for_entity(const std::string& entity_id) const override;

    void render_2d() override;
    void render_3d() override;

private:
    std::unordered_map<std::string, std::vector<contracts::OverlaySpec>> m_entityToOverlays;
};

} // namespace brain_model::core
