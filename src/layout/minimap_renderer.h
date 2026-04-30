#ifndef LAYOUT_MINIMAP_RENDERER_H
#define LAYOUT_MINIMAP_RENDERER_H

#include "../map_logic.h"
#include "../render/frame_buffer.h"

namespace render {

class MinimapRenderer {
public:
    static void render(const Graph& graph, const ViewContext& view);
    static void draw(render::FrameBuffer& fb, int x, int y, const Graph& graph, const ViewContext& view);
};

} // namespace render

#endif // LAYOUT_MINIMAP_RENDERER_H
