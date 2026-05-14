#include <iostream>
#include <fstream>
#include <filesystem>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    std::ofstream out("v24.sim");
    out << "SIM_IMG 10 10 [P]";
    out.close();
    mgr.loadImage("v24.sim");
    auto img = mgr.getActiveImage();
    std::filesystem::remove("v24.sim");
    if (img && img->isProgressive) {
        std::cout << "v24 SUCCESS: Progressive JPEG verified." << std::endl;
        return 0;
    }
    return 1;
}
