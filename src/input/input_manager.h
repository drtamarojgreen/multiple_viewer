#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "../map_logic.h"

namespace input {

class InputManager {
public:
    InputManager() = default;
    
    // Process keyboard input (delegates to existing or new logic)
    void processKeyboard(ViewContext& view, char key);
    
    // Process mouse movement/clicks
    void processMouse(ViewContext& view, int x, int y, bool leftButton, bool rightButton);

private:
    int lastMouseX_ = -1;
    int lastMouseY_ = -1;
};

} // namespace input

#endif // INPUT_MANAGER_H
