#ifndef MINIMAP_RENDERER_H
#define MINIMAP_RENDERER_H

#include "../map_logic.h"

namespace render {

class MinimapRenderer {
public:
    static void render(const Graph& graph, int screenWidth, int screenHeight);
};

} // namespace render

#endif // MINIMAP_RENDERER_H
