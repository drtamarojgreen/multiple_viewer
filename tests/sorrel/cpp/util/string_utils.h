#ifndef SORREL_STRING_UTILS_H
#define SORREL_STRING_UTILS_H

#include <string>
#include <cctype> // Required for std::isspace

namespace Sorrel {
namespace Cpp {
namespace Util {

// Function to trim whitespace from a string
std::string trim(const std::string& str);

} // namespace Util
} // namespace Cpp
} // namespace Sorrel

#endif // SORREL_STRING_UTILS_H