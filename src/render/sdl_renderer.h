#ifndef SDL_RENDERER_H
#define SDL_RENDERER_H

#include "render_interface.h"

namespace render {

class SDLRenderer : public IRenderer {
public:
    SDLRenderer() = default;
    ~SDLRenderer() override = default;

    bool initialize(int width, int height) override;
    void clear() override;
    void render(const Graph& graph, const ViewContext& view) override;
    void present() override;
    void shutdown() override;

    bool isWindowOpen() const override { return isRunning_; }

private:
    bool isRunning_ = false;
};

} // namespace render

#endif // SDL_RENDERER_H
