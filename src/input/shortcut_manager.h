#ifndef SHORTCUT_MANAGER_H
#define SHORTCUT_MANAGER_H

#include <map>
#include <string>
#include <functional>

namespace input {

class ShortcutManager {
public:
    void registerShortcut(char key, std::function<void()> action);
    void handleKey(char key);

    void setInputMode(bool enabled) { inputMode_ = enabled; }
    bool isInputMode() const { return inputMode_; }

private:
    std::map<char, std::function<void()>> shortcuts_;
    bool inputMode_ = false;
};

} // namespace input

#endif // SHORTCUT_MANAGER_H
