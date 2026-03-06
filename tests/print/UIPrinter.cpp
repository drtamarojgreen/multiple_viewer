#include "UIPrinter.h"
#include <iostream>
#include "../../src/render/visual_mapper.h"

namespace print {

bool UIPrinter::initialize(int width, int height) {
    width_ = width;
    height_ = height;
    outputBuffer_.str("");
    outputBuffer_.clear();
    return true;
}

void UIPrinter::clear() {
    outputBuffer_.str("");
    outputBuffer_.clear();
}

void UIPrinter::render(const Graph& graph, const ViewContext& view) {
    outputBuffer_ << "--- UI RENDER FRAME ---\n";
    outputBuffer_ << "Viewport: " << width_ << "x" << height_
                  << " | Pan: (" << view.panX << ", " << view.panY << ")"
                  << " | Zoom: " << static_cast<int>(view.zoomLevel)
                  << " | Mode: " << static_cast<int>(view.currentViewMode) << "\n";

    if (Config::showAnalyticsPanel) {
        outputBuffer_ << "Analytics Panel: ENABLED\n";
        outputBuffer_ << "Summary - Nodes: " << graph.nodes.size()
                      << " | Edges: " << graph.edgeCount() << "\n";
    }

    outputBuffer_ << "Minimap: " << (view.showMinimap ? "ON" : "OFF") << "\n";
    if (view.showMinimap) {
        // Output viewport box for testing purposes
        outputBuffer_ << "Viewport Box: [x=" << view.panX << ", y=" << view.panY << ", w=" << width_ << ", h=" << height_ << "]\n";
    }

    outputBuffer_ << "Focuses:";
    for (int f : graph.focusedNodeIndices) outputBuffer_ << " " << f;
    outputBuffer_ << "\n";

    outputBuffer_ << "Nodes Rendered:\n";

    for (const auto& pair : graph.nodeMap) {
        int id = pair.first;
        const auto& node = pair.second;

        int layoutX = (id * 10);
        int layoutY = (id * 5);

        int drawX = layoutX + view.panX;
        int drawY = layoutY + view.panY;

        std::string color = render::VisualMapper::getColorForWeight(node.weight);

        outputBuffer_ << "[Node: " << id << " | Name: " << node.label
                      << " | ScreenCoord: (" << drawX << ", " << drawY << ")"
                      << " | Color: " << color << "]\n";
    }

    outputBuffer_ << "Edges Rendered:\n";
    for (const auto& node : graph.nodes) {
        for (int neighbor_id : node.neighbors) {
            if (node.index < neighbor_id) {
                outputBuffer_ << "[Edge: " << node.index << " -> " << neighbor_id << "]\n";
            }
        }
    }
}

void UIPrinter::present() {}
void UIPrinter::shutdown() {}

std::string UIPrinter::getPrintedOutput() const {
    return outputBuffer_.str();
}

std::string UIPrinter::resolveNodeColor(int weight) const {
    if (weight >= 13) return "RED";
    if (weight >= 8)  return "YELLOW";
    return "CYAN";
}

} // namespace print
