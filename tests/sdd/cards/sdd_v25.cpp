#include <iostream>
#include <fstream>
#include <filesystem>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    std::ofstream out("v25.sim");
    out << "SIM_IMG 10 10 [G]";
    out.close();
    mgr.loadImage("v25.sim");
    auto img = mgr.getActiveImage();
    std::filesystem::remove("v25.sim");
    if (img && img->isAnimated) {
        std::cout << "v25 SUCCESS: GIF animation verified." << std::endl;
        return 0;
    }
    return 1;
}
