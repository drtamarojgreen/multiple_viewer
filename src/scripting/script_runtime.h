#ifndef SCRIPT_RUNTIME_H
#define SCRIPT_RUNTIME_H

#include "../map_logic.h"
#include <string>

namespace scripting {

class ScriptRuntime {
public:
    static bool executeLua(const std::string& script);
    static bool executePython(const std::string& script);
};

} // namespace scripting

#endif // SCRIPT_RUNTIME_H
