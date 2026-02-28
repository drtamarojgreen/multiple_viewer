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
                  << " | Zoom: " << static_cast<int>(view.zoomLevel) << "\n";
    outputBuffer_ << "Nodes Rendered:\n";

    // Simulate mapping logic to a text buffer
    for (const auto& pair : graph.nodeMap) {
        int id = pair.first;
        const auto& node = pair.second;

        // Simulate basic panning offset check
        // In actual UI, layout engine would do this. We capture the logical position
        // shifted by the panning context so tests can assert on the output string coordinates.

        // Base coordinate (mock for UI testing, based on index layout)
        int layoutX = (id * 10);
        int layoutY = (id * 5);

        int drawX = layoutX + view.panX;
        int drawY = layoutY + view.panY;

        std::string color = render::VisualMapper::getColorForWeight(node.weight);

        outputBuffer_ << "[Node: " << id << " | Name: " << node.label
                      << " | ScreenCoord: (" << drawX << ", " << drawY << ")"
                      << " | Color: " << color << "]\n";
    }
}

void UIPrinter::present() {
    // In headless testing, 'present' means we finished the frame recording
}

void UIPrinter::shutdown() {
}

std::string UIPrinter::getPrintedOutput() const {
    return outputBuffer_.str();
}

std::string UIPrinter::resolveNodeColor(int weight) const {
    if (weight >= 13) return "RED";
    if (weight >= 8)  return "YELLOW";
    return "CYAN";
}

} // namespace print
