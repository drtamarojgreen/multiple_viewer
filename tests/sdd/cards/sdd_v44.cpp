#include <iostream>
#include <fstream>
#include <filesystem>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    std::ofstream out("target.sim"); out << "SIM_IMG 10 10"; out.close();
    std::filesystem::create_symlink("target.sim", "link.sim");
    bool success = mgr.loadImage("link.sim");
    std::filesystem::remove("link.sim");
    std::filesystem::remove("target.sim");
    if (success) {
        std::cout << "v44 SUCCESS: Symlink resolution verified." << std::endl;
        return 0;
    }
    return 1;
}
