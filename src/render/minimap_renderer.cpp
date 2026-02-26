#include "minimap_renderer.h"
#include <iostream>
#include <vector>
#include <string>

namespace render {

void MinimapRenderer::render(const Graph& graph, const ViewContext& view) {
    // A small minimap in the corner
    int mmW = 20;
    int mmH = 10;
    std::vector<std::string> minimap(mmH, std::string(mmW, ' '));

    // Find bounds of world
    float minX = 0, maxX = 100, minY = 0, maxY = 100;
    if (!graph.nodePos.empty()) {
        minX = minY = 1e9f;
        maxX = maxY = -1e9f;
        for (const auto& [id, pos] : graph.nodePos) {
            minX = std::min(minX, pos.x); maxX = std::max(maxX, pos.x);
            minY = std::min(minY, pos.y); maxY = std::max(maxY, pos.y);
        }
    }
    float worldW = maxX - minX;
    float worldH = maxY - minY;
    if (worldW < 1) worldW = 1;
    if (worldH < 1) worldH = 1;

    auto toMapX = [&](float x) { return (int)((x - minX) / worldW * (mmW - 1)); };
    auto toMapY = [&](float y) { return (int)((y - minY) / worldH * (mmH - 1)); };

    // Render nodes
    for (const auto& [id, pos] : graph.nodePos) {
        int mx = toMapX(pos.x);
        int my = toMapY(pos.y);
        if (mx >= 0 && mx < mmW && my >= 0 && my < mmH) {
            minimap[my][mx] = graph.isNodeFocused(id) ? 'O' : '.';
        }
    }

    // Render viewport box
    int vx = toMapX(-view.panX);
    int vy = toMapY(-view.panY);
    int vw = (int)(view.width / worldW * mmW);
    int vh = (int)(view.height / worldH * mmH);
    if (vw < 1) vw = 1; if (vh < 1) vh = 1;

    for (int r = vy; r < vy + vh; ++r) {
        for (int c = vx; c < vx + vw; ++c) {
            if (r >= 0 && r < mmH && c >= 0 && c < mmW) {
                if (r == vy || r == vy + vh - 1 || c == vx || c == vx + vw - 1) {
                    if (minimap[r][c] == ' ') minimap[r][c] = '#';
                }
            }
        }
    }

    std::cout << "\n--- MINIMAP (Spatial) ---\n";
    for (const auto& row : minimap) {
        std::cout << "[" << row << "]\n";
    }
    std::cout << "-------------------------\n";
}

} // namespace render
