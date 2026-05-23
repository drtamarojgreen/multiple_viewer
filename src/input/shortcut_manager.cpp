#include "shortcut_manager.h"
#include "../io/xml_parser.h"
#include <iostream>

namespace input {

void ShortcutManager::registerShortcut(char key, const std::string& description, std::function<void()> action) {
    if (keyToIndex_.count(key)) {
        shortcutList_[keyToIndex_[key]] = {key, description, action};
    } else {
        keyToIndex_[key] = shortcutList_.size();
        shortcutList_.push_back({key, description, action});
    }
}

void ShortcutManager::registerAction(const std::string& actionId, std::function<void()> action) {
    registeredActions_[actionId] = action;
}

void ShortcutManager::loadFromXml(const std::string& filepath) {
    auto root = io::XmlParser::loadFile(filepath);
    for (const auto& node : root.children) {
        if (node.name == "Command") {
            std::string keyStr = node.attributes.count("key") ? node.attributes.at("key") : "";
            std::string actionId = node.attributes.count("action") ? node.attributes.at("action") : "";
            std::string desc = node.attributes.count("description") ? node.attributes.at("description") : "";

            if (keyStr.empty() || actionId.empty()) continue;

            char keyChar = 0;
            if (keyStr == "TAB") keyChar = '\t';
            else if (keyStr.length() == 1) keyChar = keyStr[0];

            if (keyChar != 0 && registeredActions_.count(actionId)) {
                registerShortcut(keyChar, desc, registeredActions_.at(actionId));
            }
        }
    }
}

void ShortcutManager::handleKey(char key) {
    if (keyToIndex_.count(key)) {
        shortcutList_[keyToIndex_[key]].action();
    }
}

} // namespace input
