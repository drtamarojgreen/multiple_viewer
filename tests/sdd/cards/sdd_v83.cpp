#include <iostream>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    if (mgr.checkLifecycle("v83")) {
        std::cout << "v83 SUCCESS: Race condition prevention verified." << std::endl;
        return 0;
    }
    return 1;
}
