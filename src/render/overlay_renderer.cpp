#include "overlay_renderer.h"
#include "model/overlay_manager.h"
#include "frame_buffer.h"
#include "viewport.h"
#include "map_logic.h"
#include <cmath>

namespace render {

OverlayRenderer::OverlayRenderer() {
}

void OverlayRenderer::renderOverlays(const model::OverlayManager& overlayMgr,
                                      const Graph& networkGraph,
                                      FrameBuffer& frameBuf,
                                      const ViewContext& view,
                                      const Viewport& viewport) {
    // First pass: render overlay markers
    std::vector<int> networkNodes = networkGraph.getAllNodeIds();

    for (int nodeId : networkNodes) {
        if (!overlayMgr.isOverlayNode(nodeId)) continue;

        auto pos = networkGraph.getNodePosition(nodeId);
        Point2D screenPos = viewport.worldToScreen(pos.x, pos.y);

        renderOverlayMarker(frameBuf, static_cast<int>(screenPos.x), static_cast<int>(screenPos.y));
    }

    // Second pass: render overlay edges
    // For now, we only render markers as the edge rendering logic needs more
    // access to overlay graph positions which aren't readily available in this loop
    // without potentially expensive lookups or better structure.
}

void OverlayRenderer::renderOverlayMarker(FrameBuffer& frameBuf, int x, int y) {
    // Overwrite with '#' character at position
    frameBuf.writeAt(x, y, '#');
}

void OverlayRenderer::renderOverlayEdge(FrameBuffer& frameBuf,
                                         int fromX, int fromY,
                                         int toX, int toY) {
    drawDottedLine(frameBuf, fromX, fromY, toX, toY);
}

void OverlayRenderer::drawDottedLine(FrameBuffer& frameBuf,
                                      int x0, int y0, int x1, int y1) {
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    int x = x0;
    int y = y0;
    int step = 0;

    int width = frameBuf.getWidth();
    int height = frameBuf.getHeight();

    while (true) {
        // Draw dot every other step
        if (step % 2 == 0) {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                char existing = frameBuf.getChar(x, y);
                if (existing == ' ' || existing == '.') {
                    frameBuf.writeAt(x, y, '.');
                }
            }
        }

        if (x == x1 && y == y1) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }

        step++;
        if (step > 1000) break; // Safety break
    }
}

} // namespace render
