#include <iostream>
#include <fstream>
#include <filesystem>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    std::ofstream out("v23.sim");
    out << "SIM_IMG 10 10 [A]";
    out.close();
    mgr.loadImage("v23.sim");
    auto img = mgr.getActiveImage();
    std::filesystem::remove("v23.sim");
    if (img && img->hasAlpha) {
        std::cout << "v23 SUCCESS: Alpha channel verified." << std::endl;
        return 0;
    }
    return 1;
}
