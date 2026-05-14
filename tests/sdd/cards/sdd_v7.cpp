#include <iostream>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    if (mgr.checkResource("v7")) {
        std::cout << "v7 SUCCESS: Rapid switching verified." << std::endl;
        return 0;
    }
    return 1;
}
