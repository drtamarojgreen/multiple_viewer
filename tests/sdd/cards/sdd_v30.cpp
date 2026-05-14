#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    std::vector<std::filesystem::path> paths = {"v30a.sim", "v30b.sim"};
    for(auto& p : paths) { std::ofstream out(p); out << "SIM_IMG 10 10"; out.close(); }
    if (mgr.handleDrop(paths)) {
        std::cout << "v30 SUCCESS: Drag-and-drop verified." << std::endl;
        for(auto& p : paths) std::filesystem::remove(p);
        return 0;
    }
    for(auto& p : paths) std::filesystem::remove(p);
    return 1;
}
