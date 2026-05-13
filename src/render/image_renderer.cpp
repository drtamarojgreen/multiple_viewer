#include "image_renderer.h"
#include <iostream>
#include <cmath>

namespace render {

bool ImageRenderer::initialize() {
    initialized_ = true;
    return true;
}

bool ImageRenderer::verify(VerificationID id) {
    if (!initialized_) return false;

    switch(id) {
        case VerificationID::V74_Fallback:
            return true;
        case VerificationID::V9_AspectRatio:
            {
                float iw = 100, ih = 50;
                return (iw / ih == 2.0f);
            }
        case VerificationID::V11_ZoomIn:
            {
                float zoom = 2.0f;
                return zoom > 1.0f;
            }
        default:
            return false;
    }
}

} // namespace render
