#include <iostream>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    if (mgr.checkResource("v89")) {
        std::cout << "v89 SUCCESS: Clipboard paste rejection verified." << std::endl;
        return 0;
    }
    return 1;
}
