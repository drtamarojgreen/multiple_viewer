#ifndef CONSOLE_RENDERER_H
#define CONSOLE_RENDERER_H

#include "render_interface.h"

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

private:
    int width_ = 80;
    int height_ = 25;
};

} // namespace render

#endif // CONSOLE_RENDERER_H
