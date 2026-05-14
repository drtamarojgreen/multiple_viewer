#include <iostream>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    if (mgr.checkResource("v6")) {
        std::cout << "v6 SUCCESS: Large image handling verified." << std::endl;
        return 0;
    }
    return 1;
}
