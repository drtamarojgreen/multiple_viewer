#include <iostream>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    if (mgr.checkLifecycle("v50")) {
        std::cout << "v50 SUCCESS: Startup time verified." << std::endl;
        return 0;
    }
    return 1;
}
