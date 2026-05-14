#include <iostream>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    if (mgr.checkLifecycle("v51")) {
        std::cout << "v51 SUCCESS: Shutdown verified." << std::endl;
        return 0;
    }
    return 1;
}
