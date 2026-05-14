#include <iostream>
#include <fstream>
#include <filesystem>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    std::ofstream out("v8.sim");
    out << "SIM_IMG 10 10";
    out.close();
    mgr.loadImage("v8.sim");
    mgr.closeImage();
    std::filesystem::remove("v8.sim");
    if (!mgr.getActiveImage()) {
        std::cout << "v8 SUCCESS: Buffer disposed." << std::endl;
        return 0;
    }
    return 1;
}
