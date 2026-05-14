#include <iostream>
#include <fstream>
#include <filesystem>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    std::ofstream out("v17.sim");
    out << "SIM_IMG 10 10";
    out.close();
    mgr.loadImage("v17.sim");
    if (mgr.flipVertical()) {
        std::cout << "v17 SUCCESS: Vertical flip verified." << std::endl;
        std::filesystem::remove("v17.sim");
        return 0;
    }
    std::filesystem::remove("v17.sim");
    return 1;
}
