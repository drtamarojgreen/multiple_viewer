#include "frame_buffer.h"
#include <iostream>
#include <algorithm>

namespace render {

FrameBuffer::FrameBuffer(int width, int height)
    : width_(width), height_(height),
      buffer_(height, std::string(width, ' ')),
      depthBuffer_(height, std::vector<float>(width, std::numeric_limits<float>::infinity())) {}

void FrameBuffer::clear() {
    for (auto& row : buffer_) {
        std::fill(row.begin(), row.end(), ' ');
    }
    for (auto& row : depthBuffer_) {
        std::fill(row.begin(), row.end(), std::numeric_limits<float>::infinity());
    }
    title_.clear();
    statusMessage_.clear();
}

void FrameBuffer::drawChar(int x, int y, char c, float depth) {
    if (x >= 0 && x < width_ && y >= 0 && y < height_) {
        if (depth <= depthBuffer_[y][x]) {
            buffer_[y][x] = c;
            depthBuffer_[y][x] = depth;
        }
    }
}

void FrameBuffer::drawString(int x, int y, const std::string& str, float depth) {
    for (size_t i = 0; i < str.length(); ++i) {
        drawChar(x + static_cast<int>(i), y, str[i], depth);
    }
}

void FrameBuffer::drawLine(int x1, int y1, int x2, int y2, char c, float depth) {
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        drawChar(x1, y1, c, depth);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void FrameBuffer::drawRect(int x, int y, int w, int h, char c, float depth) {
    for (int i = 0; i < w; ++i) {
        drawChar(x + i, y, c, depth);
        drawChar(x + i, y + h - 1, c, depth);
    }
    for (int i = 1; i < h - 1; ++i) {
        drawChar(x, y + i, c, depth);
        drawChar(x + w - 1, y + i, c, depth);
    }
}

void FrameBuffer::setTitle(const std::string& title) {
    title_ = title;
}

void FrameBuffer::setStatusMessage(const std::string& message) {
    statusMessage_ = message;
}

void FrameBuffer::present() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    if (!title_.empty()) {
        std::cout << "=== " << title_ << " ===\n";
    }
    for (const auto& row : buffer_) {
        std::cout << row << "\n";
    }
    if (!statusMessage_.empty()) {
        std::cout << "Status: " << statusMessage_ << "\n";
    }
}

} // namespace render
