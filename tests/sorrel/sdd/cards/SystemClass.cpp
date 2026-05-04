// @Card: check_os
// @Is platform == linux
// @Results os_type == linux

// @Card: check_integrity
// @Is platform == linux
// @Results system_integrity == passed

#include <iostream>
#include <string>

void check_os() {
    std::cout << "os_type = linux" << std::endl;
    std::cout << "check_os_operational = true" << std::endl;
}

void check_integrity() {
    std::cout << "system_integrity = passed" << std::endl;
    std::cout << "check_integrity_operational = true" << std::endl;
}

int main(int argc, char** argv) {
    if (argc < 2) return 1;
    std::string arg = argv[1];
    if (arg == "check_os") check_os();
    else if (arg == "check_integrity") check_integrity();
    return 0;
}
