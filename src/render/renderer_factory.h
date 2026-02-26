#ifndef RENDERER_FACTORY_H
#define RENDERER_FACTORY_H

#include "render_interface.h"
#include "console_renderer.h"
#include "sdl_renderer.h"
#include <memory>
#include <string>

namespace render {

enum class RendererType {
    CONSOLE,
    SDL,
    OPENGL
};

class RendererFactory {
public:
    static std::unique_ptr<IRenderer> createRenderer(RendererType type) {
        switch (type) {
            case RendererType::CONSOLE:
                return std::make_unique<ConsoleRenderer>();
            case RendererType::SDL:
                return std::make_unique<SDLRenderer>();
            default:
                return std::make_unique<ConsoleRenderer>();
        }
    }
};

} // namespace render

#endif // RENDERER_FACTORY_H
