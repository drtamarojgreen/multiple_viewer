#include "image_renderer.h"
#include <iostream>

namespace render {

bool ImageRenderer::initialize() {
    std::cout << "[ImageRenderer] Init Software path" << std::endl;
    path_ = RenderPath::Software;
    return true;
}

bool ImageRenderer::testLayout(const std::string& feature) { return false; }
bool ImageRenderer::testZoom(float level) { return false; }
bool ImageRenderer::testPanning(int x, int y) { return false; }
bool ImageRenderer::applyFilter(const std::string& name) { return false; }
bool ImageRenderer::testAnalytics(const std::string& metric) { return false; }
bool ImageRenderer::testInput(const std::string& action) { return false; }
bool ImageRenderer::testLocalization(const std::string& key) { return false; }
bool ImageRenderer::testRenderQueue(const std::string& mode) { return false; }
bool ImageRenderer::testGPU(const std::string& state) { return false; }
bool ImageRenderer::testPerformance(const std::string& metric) { return false; }

} // namespace render
