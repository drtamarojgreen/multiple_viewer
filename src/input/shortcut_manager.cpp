#include "shortcut_manager.h"

namespace input {

void ShortcutManager::registerShortcut(char key, std::function<void()> action) {
    shortcuts_[key] = action;
}

void ShortcutManager::handleKey(char key) {
    if (shortcuts_.count(key)) {
        shortcuts_[key]();
    }
}

} // namespace input
