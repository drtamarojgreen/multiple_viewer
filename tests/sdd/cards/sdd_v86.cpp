#include <iostream>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    if (mgr.checkLifecycle("v86")) {
        std::cout << "v86 SUCCESS: Screenshot export verified." << std::endl;
        return 0;
    }
    return 1;
}
