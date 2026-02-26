#include "sdl_renderer.h"
#include <iostream>

namespace render {

bool SDLRenderer::initialize(int width, int height) {
    std::cout << "[SDL] Initializing SDL backend (Stub)\n";
    isRunning_ = true;
    return true;
}

void SDLRenderer::clear() {
    // SDL clear logic
}

void SDLRenderer::render(const Graph& graph, const ViewContext& view) {
    // SDL rendering logic would go here
}

void SDLRenderer::present() {
    // SDL present logic
}

void SDLRenderer::shutdown() {
    std::cout << "[SDL] Shutting down SDL backend\n";
    isRunning_ = false;
}

} // namespace render
