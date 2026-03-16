#include <iostream>
#include <string>
// Include the new utility header
#include "cpp/util/string_utils.h"

int main() {
    std::string test1 = "  hello  ";
    std::string test2 = "hello";
    std::string test3 = "  hello world  ";
    std::string test4 = "";
    std::string test5 = "     ";
    // Corrected string literal for test6
    std::string test6 = "\t\nleading and trailing\r\n";

    bool operational = true; // Declare operational here

    if (Chai::Cpp::Util::trim(test1) != "hello") operational = false;
    if (Chai::Cpp::Util::trim(test2) != "hello") operational = false;
    if (Chai::Cpp::Util::trim(test3) != "hello world") operational = false;
    if (Chai::Cpp::Util::trim(test4) != "") operational = false;
    if (Chai::Cpp::Util::trim(test5) != "") operational = false;
    if (Chai::Cpp::Util::trim(test6) != "leading and trailing") operational = false;

    std::cout << "string_trim_operational = " << (operational ? "true" : "false") << std::endl;

    return 0;
}

