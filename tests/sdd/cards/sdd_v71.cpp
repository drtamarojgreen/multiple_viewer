#include <iostream>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    if (mgr.checkPlugin("v71")) {
        std::cout << "v71 SUCCESS: Plugin loader rejected invalid." << std::endl;
        return 0;
    }
    return 1;
}
