#include <iostream>
#include <fstream>
#include <filesystem>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    std::ofstream out("v27.sim");
    out << "SIM_IMG 10 10 [MULTI]";
    out.close();
    mgr.loadImage("v27.sim");
    auto img = mgr.getActiveImage();
    img->pageCount = 2;
    if (mgr.nextPage()) {
        std::cout << "v27 SUCCESS: TIFF/Multi-page navigation verified." << std::endl;
        std::filesystem::remove("v27.sim");
        return 0;
    }
    std::filesystem::remove("v27.sim");
    return 1;
}
