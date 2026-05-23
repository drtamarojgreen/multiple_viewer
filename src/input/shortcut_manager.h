#ifndef SHORTCUT_MANAGER_H
#define SHORTCUT_MANAGER_H

#include <map>
#include <string>
#include <functional>
#include <vector>
#include <unordered_map>

namespace input {

struct Shortcut {
    char key;
    std::string description;
    std::function<void()> action;
};

class ShortcutManager {
public:
    void registerShortcut(char key, const std::string& description, std::function<void()> action);
    void registerAction(const std::string& actionId, std::function<void()> action);
    void loadFromXml(const std::string& filepath);
    void handleKey(char key);

    void setInputMode(bool enabled) { inputMode_ = enabled; }
    bool isInputMode() const { return inputMode_; }

    const std::vector<Shortcut>& getShortcuts() const { return shortcutList_; }

private:
    std::map<char, size_t> keyToIndex_;
    std::vector<Shortcut> shortcutList_;
    std::unordered_map<std::string, std::function<void()>> registeredActions_;
    bool inputMode_ = false;
};

} // namespace input

#endif // SHORTCUT_MANAGER_H
