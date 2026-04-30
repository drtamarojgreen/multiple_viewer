#include "shortcut_manager.h"

namespace input {

void ShortcutManager::registerShortcut(char key, const std::string& description, std::function<void()> action) {
    if (keyToIndex_.count(key)) {
        shortcutList_[keyToIndex_[key]] = {key, description, action};
    } else {
        keyToIndex_[key] = shortcutList_.size();
        shortcutList_.push_back({key, description, action});
    }
}

void ShortcutManager::handleKey(char key) {
    if (keyToIndex_.count(key)) {
        shortcutList_[keyToIndex_[key]].action();
    }
}

} // namespace input
