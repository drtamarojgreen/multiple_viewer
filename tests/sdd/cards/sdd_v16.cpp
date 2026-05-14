#include <iostream>
#include <fstream>
#include <filesystem>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    std::ofstream out("v16.sim");
    out << "SIM_IMG 10 10";
    out.close();
    mgr.loadImage("v16.sim");
    if (mgr.flip(true)) {
        std::cout << "v16 SUCCESS: Horizontal flip verified." << std::endl;
        std::filesystem::remove("v16.sim");
        return 0;
    }
    std::filesystem::remove("v16.sim");
    return 1;
}
