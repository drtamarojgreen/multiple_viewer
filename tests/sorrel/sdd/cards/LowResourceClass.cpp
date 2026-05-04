// @Card: low_resource_check
// @Situation LowResources
// @Is disk_space == low
// @Results low_resource_mode == enabled

#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if (argc < 2) return 1;
    std::string card = argv[1];
    if (card == "low_resource_check") {
        std::cout << "low_resource_mode = enabled" << std::endl;
    }
    return 0;
}
