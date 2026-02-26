#include "camera.h"

namespace render {

void Camera::update(float deltaTime) {
    // Basic linear interpolation for smooth movement
    currentX_ += (targetX_ - currentX_) * lerpSpeed_ * deltaTime;
    currentY_ += (targetY_ - currentY_) * lerpSpeed_ * deltaTime;
    currentZoom_ += (targetZoom_ - currentZoom_) * lerpSpeed_ * deltaTime;
}

void Camera::setTargetPosition(float x, float y) {
    targetX_ = x;
    targetY_ = y;
}

void Camera::setTargetZoom(float zoom) {
    targetZoom_ = zoom;
}

} // namespace render
