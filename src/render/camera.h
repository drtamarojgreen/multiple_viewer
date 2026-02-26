#ifndef CAMERA_H
#define CAMERA_H

namespace render {

class Camera {
public:
    Camera() = default;

    void update(float deltaTime);

    void setTargetPosition(float x, float y);
    void setTargetZoom(float zoom);

    float getX() const { return currentX_; }
    float getY() const { return currentY_; }
    float getZoom() const { return currentZoom_; }

private:
    float currentX_ = 0.0f;
    float currentY_ = 0.0f;
    float currentZoom_ = 1.0f;

    float targetX_ = 0.0f;
    float targetY_ = 0.0f;
    float targetZoom_ = 1.0f;

    float lerpSpeed_ = 5.0f;
};

} // namespace render

#endif // CAMERA_H
