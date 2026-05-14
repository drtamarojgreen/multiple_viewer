#include "path_validator.h"
#include <iostream>
#include <filesystem>

namespace io {

bool PathValidator::isSafe(const std::filesystem::path& path, const std::filesystem::path& baseDir) {
    try {
        // Correct path containment logic for Linux
        auto absPath = std::filesystem::absolute(path).lexically_normal();
        auto absBase = std::filesystem::absolute(baseDir).lexically_normal();

        std::string sPath = absPath.string();
        std::string sBase = absBase.string();

        // Ensure sBase has trailing slash to avoid partial match (e.g. /app vs /apple)
        if (sBase.back() != std::filesystem::path::preferred_separator) {
            sBase += std::filesystem::path::preferred_separator;
        }

        // Base directory containment
        if (sPath.find(sBase) != 0) {
            return false;
        }

        // Prohibit sensitive system paths
        if (sPath.find("/etc/") != std::string::npos || sPath.find("/var/") != std::string::npos) {
            return false;
        }
        return true;
    } catch (...) {
        return false;
    }
}

} // namespace io
