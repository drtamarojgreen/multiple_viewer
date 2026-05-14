#include <iostream>
#include <fstream>
#include <filesystem>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    std::ofstream out("v26.sim");
    out << "SIM_IMG 10 10 [G]";
    out.close();
    mgr.loadImage("v26.sim");
    mgr.togglePlayback();
    auto img = mgr.getActiveImage();
    std::filesystem::remove("v26.sim");
    if (img && img->isPlaying) {
        std::cout << "v26 SUCCESS: GIF control verified." << std::endl;
        return 0;
    }
    return 1;
}
