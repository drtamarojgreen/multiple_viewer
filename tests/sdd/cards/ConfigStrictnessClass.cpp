#include <iostream>
#include <map>
#include "map_logic.h"

int main() {
    // Uninitialized globals in BSS should be 0.
    std::cout << "config_strictness_operational = " << ((Config::consoleWidth == 0) ? "true" : "false") << std::endl;
    return 0;
}
