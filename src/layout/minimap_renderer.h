#ifndef LAYOUT_MINIMAP_RENDERER_H
#define LAYOUT_MINIMAP_RENDERER_H

#include "../map_logic.h"

namespace render {

class MinimapRenderer {
public:
    static void render(const Graph& graph, const ViewContext& view);
};

} // namespace render

#endif // LAYOUT_MINIMAP_RENDERER_H
