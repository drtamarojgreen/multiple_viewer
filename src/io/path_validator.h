#ifndef PATH_VALIDATOR_H
#define PATH_VALIDATOR_H

#include <filesystem>

namespace io {

class PathValidator {
public:
    static bool isSafe(const std::filesystem::path& path, const std::filesystem::path& baseDir);
};

} // namespace io

#endif
