// @Is platform == linux
// @Results os_type == linux
#include <iostream>

int main() {
    #ifdef __linux__
        std::cout << "os_type = linux" << std::endl;
    #elif _WIN32
        std::cout << "os_type = windows" << std::endl;
    #else
        std::cout << "os_type = unknown" << std::endl;
    #endif
    std::cout << "check_os_operational = true" << std::endl;
    return 0;
}
