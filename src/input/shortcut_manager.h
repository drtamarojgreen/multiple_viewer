#ifndef SHORTCUT_MANAGER_H
#define SHORTCUT_MANAGER_H

#include <map>
#include <string>
#include <functional>
#include <vector>

namespace input {

struct Shortcut {
    char key;
    std::string description;
    std::function<void()> action;
};

class ShortcutManager {
public:
    void registerShortcut(char key, const std::string& description, std::function<void()> action);
    void handleKey(char key);

    void setInputMode(bool enabled) { inputMode_ = enabled; }
    bool isInputMode() const { return inputMode_; }

    const std::vector<Shortcut>& getShortcuts() const { return shortcutList_; }

private:
    std::map<char, size_t> keyToIndex_;
    std::vector<Shortcut> shortcutList_;
    bool inputMode_ = false;
};

} // namespace input

#endif // SHORTCUT_MANAGER_H
