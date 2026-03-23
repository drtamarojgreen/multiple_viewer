#ifndef CONSOLE_RENDERER_H
#define CONSOLE_RENDERER_H

#include "render_interface.h"
#include "frame_buffer.h"
#include "viewport.h"
#include <memory>

namespace render {

class ConsoleRenderer : public IRenderer {
public:
    ConsoleRenderer() = default;
    ~ConsoleRenderer() override = default;

    bool initialize(int width, int height) override;
    void clear() override;
    void render(const Graph& graph, const ViewContext& view) override;
    void present() override;
    void shutdown() override;

    bool isWindowOpen() const override { return true; } // Console is always "open"
    void setStatusMessage(const std::string& message) override;

private:
    int width_ = 80;
    int height_ = 25;
    std::unique_ptr<FrameBuffer> frameBuffer_;
    std::unique_ptr<Viewport> viewport_;
};

} // namespace render

#endif // CONSOLE_RENDERER_H
