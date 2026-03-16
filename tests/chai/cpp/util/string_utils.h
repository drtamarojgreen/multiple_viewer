#ifndef CHAI_STRING_UTILS_H
#define CHAI_STRING_UTILS_H

#include <string>
#include <cctype> // Required for std::isspace

namespace Chai {
namespace Cpp {
namespace Util {

// Function to trim whitespace from a string
std::string trim(const std::string& str);

} // namespace Util
} // namespace Cpp
} // namespace Chai

#endif // CHAI_STRING_UTILS_H