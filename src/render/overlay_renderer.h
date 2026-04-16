#ifndef OVERLAY_RENDERER_H
#define OVERLAY_RENDERER_H

#include <vector>

namespace model {
    class OverlayManager;
}

#include "../map_logic.h"

namespace render {

class FrameBuffer;
class Viewport;

// Renders overlay markers and edges on top of base graph
class OverlayRenderer {
public:
    OverlayRenderer();

    // Render all overlays to frame buffer
    // Must be called AFTER base graph rendering
    void renderOverlays(const model::OverlayManager& overlayMgr,
                        const Graph& networkGraph,
                        FrameBuffer& frameBuf,
                        const ViewContext& view,
                        const Viewport& viewport);

private:
    void renderOverlayMarker(FrameBuffer& frameBuf, int x, int y);

    void renderOverlayEdge(FrameBuffer& frameBuf,
                           int fromX, int fromY,
                           int toX, int toY);

    // Bresenham line for dot edge rendering
    void drawDottedLine(FrameBuffer& frameBuf,
                        int x0, int y0, int x1, int y1);
};

} // namespace render

#endif // OVERLAY_RENDERER_H
