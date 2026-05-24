#include "recovery_manager.h"
#include "io_manager.h"
#include "../logger.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace io {

std::string RecoveryManager::autosavePath_;
bool RecoveryManager::isDirty_ = false;

void RecoveryManager::initialize(const std::string& autosavePath) {
    autosavePath_ = autosavePath;
}

void RecoveryManager::triggerAutosave(const Graph& graph) {
    // Only save if the graph has changed (minimal logic for now)
    IOManager::saveGraphToCSV(graph, autosavePath_);
    Logger::info("Autosave triggered: " + autosavePath_);
}

bool RecoveryManager::detectAndRestore(Graph& graph) {
    if (fs::exists(autosavePath_)) {
        Logger::info("Crash recovery: Autosave file detected. Restoring state...");
        return IOManager::loadGraphFromCSV(graph, autosavePath_);
    }
    return false;
}

} // namespace io
