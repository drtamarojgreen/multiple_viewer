#include <iostream>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    if (mgr.checkPlugin("v72")) {
        std::cout << "v72 SUCCESS: Plugin failure handled." << std::endl;
        return 0;
    }
    return 1;
}
