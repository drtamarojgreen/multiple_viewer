#include "minimap_renderer.h"
#include <iostream>
#include <vector>
#include <string>

namespace render {

void MinimapRenderer::render(const Graph& graph, int screenWidth, int screenHeight) {
    // A small minimap in the corner
    int mmW = 15;
    int mmH = 7;
    std::vector<std::string> minimap(mmH, std::string(mmW, ' '));
    
    for (const auto& node : graph.nodes) {
        // Simple scaling
        int mx = (node.index % mmW);
        int my = (node.index / mmW) % mmH;
        if (mx >= 0 && mx < mmW && my >= 0 && my < mmH) {
            minimap[my][mx] = graph.isNodeFocused(node.index) ? 'O' : '.';
        }
    }
    
    std::cout << "\n--- MINIMAP ---\n";
    for (const auto& row : minimap) {
        std::cout << "[" << row << "]\n";
    }
    std::cout << "---------------\n";
}

} // namespace render
