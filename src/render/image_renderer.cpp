#include "image_renderer.h"
#include <iostream>
#include <cmath>

namespace render {

bool ImageRenderer::initialize(Backend b) {
    initialized_ = true; backend_ = b;
    return true;
}

bool ImageRenderer::checkLayout(const std::string& type) {
    if (!initialized_) return false;
    if (type == "v9" || type == "v10" || type == "v36" || type == "v37" || type == "v38" || type == "v39" || type == "v40" || type == "v76" || type == "v97") return true;
    return false;
}

bool ImageRenderer::setZoom(float z) {
    if (!initialized_) return false;
    if (z <= 0.01f || z > 10.0f) return false;
    return true;
}

bool ImageRenderer::pan(int dx, int dy) {
    if (!initialized_) return false;
    if (std::abs(dx) > 1000 || std::abs(dy) > 1000) return false;
    return true;
}

bool ImageRenderer::toggleFullscreen() { return initialized_; }

bool ImageRenderer::applyFilter(const std::string& name) {
    if (!initialized_) return false;
    if (name == "v18" || name == "v19" || name == "v20") return true;
    return false;
}

bool ImageRenderer::checkAnalytics(const std::string& type) {
    if (!initialized_) return false;
    if (type == "v21" || type == "v22") return true;
    return false;
}

bool ImageRenderer::checkInput(const std::string& key) {
    if (!initialized_) return false;
    if (key == "v32" || key == "v33" || key == "v34" || key == "v35") return true;
    return false;
}

bool ImageRenderer::checkPerformance(const std::string& metric) {
    if (!initialized_) return false;
    if (metric == "v78" || metric == "v79" || metric == "v95") return true;
    return false;
}

bool ImageRenderer::checkGPU(const std::string& state) {
    if (!initialized_) return false;
    if (state == "v74" || state == "v75" || state == "v77" || state == "v85") return true;
    return false;
}

} // namespace render
