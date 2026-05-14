#include <iostream>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    if (mgr.checkResource("v93")) {
        std::cout << "v93 SUCCESS: Decompression bomb rejected." << std::endl;
        return 0;
    }
    return 1;
}
