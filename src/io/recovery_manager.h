#ifndef RECOVERY_MANAGER_H
#define RECOVERY_MANAGER_H

#include <string>
#include "../map_logic.h"

namespace io {

class RecoveryManager {
public:
    static void initialize(const std::string& autosavePath);
    static void triggerAutosave(const Graph& graph);
    static bool detectAndRestore(Graph& graph);

private:
    static std::string autosavePath_;
    static bool isDirty_;
};

} // namespace io

#endif // RECOVERY_MANAGER_H
