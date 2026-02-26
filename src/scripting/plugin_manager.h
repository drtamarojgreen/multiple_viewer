#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <vector>
#include <string>
#include <memory>

namespace scripting {

class IPlugin {
public:
    virtual ~IPlugin() = default;
    virtual void onInit() = 0;
    virtual void onUpdate() = 0;
};

class PluginManager {
public:
    void loadPlugin(const std::string& path);
    void updatePlugins();

private:
    std::vector<std::unique_ptr<IPlugin>> plugins_;
};

} // namespace scripting

#endif // PLUGIN_MANAGER_H
