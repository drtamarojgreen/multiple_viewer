#ifndef RENDER_LAYER_MANAGER_H
#define RENDER_LAYER_MANAGER_H

#include <vector>
#include <string>
#include <map>

namespace render {

enum class LayerType {
    SURFACE,
    NODE,
    EDGE,
    SCALAR_FIELD,
    OVERLAY,
    GLYPH,
    ANNOTATION
};

struct RenderLayer {
    std::string id;
    std::string name;
    LayerType type;
    bool visible = true;
    float opacity = 1.0f;
    int zOrder = 0;
};

class RenderLayerManager {
public:
    static RenderLayerManager& getInstance() {
        static RenderLayerManager instance;
        return instance;
    }

    void addLayer(const RenderLayer& layer);
    void removeLayer(const std::string& id);

    void setVisibility(const std::string& id, bool visible);
    bool isVisible(const std::string& id) const;

    std::vector<RenderLayer> getSortedLayers() const;

    // Depth rules
    void setLayerDepthMode(const std::string& id, bool depthAware);

private:
    RenderLayerManager() = default;
    std::map<std::string, RenderLayer> layers_;
};

} // namespace render

#endif // RENDER_LAYER_MANAGER_H
