#include "plugin_manager.h"
#include <iostream>

namespace scripting {

void PluginManager::loadPlugin(const std::string& path) {
    std::cout << "[Plugin] Loading: " << path << "\n";
    // Future: dlopen/LoadLibrary logic
}

void PluginManager::updatePlugins() {
    for (auto& plugin : plugins_) {
        plugin->onUpdate();
    }
}

} // namespace scripting
