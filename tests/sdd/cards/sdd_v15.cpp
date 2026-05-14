#include <iostream>
#include <fstream>
#include <filesystem>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    std::ofstream out("v15.sim");
    out << "SIM_IMG 10 20"; // W=10, H=20
    out.close();
    mgr.loadImage("v15.sim");
    mgr.rotate(90);
    auto img = mgr.getActiveImage();
    std::filesystem::remove("v15.sim");
    if (img && img->width == 20 && img->height == 10) {
        std::cout << "v15 SUCCESS: Rotation swapped dimensions." << std::endl;
        return 0;
    }
    return 1;
}
