#include "string_utils.h"
#include <algorithm> // Not strictly needed for this trim, but common for string algos

namespace Chai {
namespace Cpp {
namespace Util {

std::string trim(const std::string& str) {
    // Corrected string literal for whitespace characters
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (first == std::string::npos) {
        return ""; // String is all whitespace or empty
    }
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}

} // namespace Util
} // namespace Cpp
} // namespace Chai

