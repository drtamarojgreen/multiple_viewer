#include "input_manager.h"

namespace input {

void InputManager::processKeyboard(ViewContext& view, char key) {
    // Basic navigation mapping
    switch (key) {
        case 'i': case 'I': view.pan(0, -1); break;
        case 'k': case 'K': view.pan(0, 1); break;
        case 'j': case 'J': view.pan(-1, 0); break;
        case 'l': case 'L': view.pan(1, 0); break;
        case 'z': case 'Z': view.zoomIn(); break;
        case 'x': case 'X': view.zoomOut(); break;
    }
}

void InputManager::processMouse(ViewContext& view, int x, int y, bool leftButton, bool rightButton) {
    if (lastMouseX_ != -1 && lastMouseY_ != -1) {
        if (leftButton) {
            // Pan with mouse
            int dx = x - lastMouseX_;
            int dy = y - lastMouseY_;
            view.pan(dx, dy);
        }
    }
    lastMouseX_ = x;
    lastMouseY_ = y;
}

} // namespace input
