#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <vector>
#include <string>
#include <limits>

namespace render {

class FrameBuffer {
public:
    FrameBuffer(int width, int height);

    void clear();
    void drawChar(int x, int y, char c, float depth = 0.0f);
    void drawString(int x, int y, const std::string& str, float depth = 0.0f);
    void drawLine(int x1, int y1, int x2, int y2, char c, float depth = 0.0f);
    void drawRect(int x, int y, int w, int h, char c, float depth = 0.0f);

    void setTitle(const std::string& title);
    void setStatusMessage(const std::string& message);
    void present() const;

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

private:
    int width_;
    int height_;
    std::vector<std::string> buffer_;
    std::vector<std::vector<float>> depthBuffer_;
    std::string title_;
    std::string statusMessage_;
};

} // namespace render

#endif // FRAME_BUFFER_H
