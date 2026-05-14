#include <iostream>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    if (mgr.checkConfig("v62")) {
        std::cout << "v62 SUCCESS: Malformed config rejected." << std::endl;
        return 0;
    }
    return 1;
}
