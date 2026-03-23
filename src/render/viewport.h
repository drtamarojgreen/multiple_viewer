#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "../map_logic.h"

namespace render {

class Viewport {
public:
    Viewport(int width, int height) : width_(width), height_(height), panX_(0), panY_(0), zoom_(1.0f) {}

    void setPan(int x, int y) { panX_ = x; panY_ = y; }
    void setZoom(float zoom) { zoom_ = zoom; }

    Point2D worldToScreen(float x, float y) const {
        return { (x + panX_) * zoom_, (y + panY_) * zoom_ };
    }

    bool isVisible(int x, int y) const {
        return x >= 0 && x < width_ && y >= 0 && y < height_;
    }

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

private:
    int width_;
    int height_;
    int panX_;
    int panY_;
    float zoom_;
};

} // namespace render

#endif // VIEWPORT_H
