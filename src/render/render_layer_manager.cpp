#include "render_layer_manager.h"
#include <algorithm>

namespace render {

void RenderLayerManager::addLayer(const RenderLayer& layer) {
    layers_[layer.id] = layer;
}

void RenderLayerManager::removeLayer(const std::string& id) {
    layers_.erase(id);
}

void RenderLayerManager::setVisibility(const std::string& id, bool visible) {
    if (layers_.count(id)) {
        layers_[id].visible = visible;
    }
}

bool RenderLayerManager::isVisible(const std::string& id) const {
    auto it = layers_.find(id);
    if (it != layers_.end()) {
        return it->second.visible;
    }
    return false;
}

std::vector<RenderLayer> RenderLayerManager::getSortedLayers() const {
    std::vector<RenderLayer> result;
    for (const auto& pair : layers_) {
        result.push_back(pair.second);
    }
    std::sort(result.begin(), result.end(), [](const RenderLayer& a, const RenderLayer& b) {
        return a.zOrder < b.zOrder;
    });
    return result;
}

} // namespace render
