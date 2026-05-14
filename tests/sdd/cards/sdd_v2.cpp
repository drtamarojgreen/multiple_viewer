#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    std::vector<std::string> files = {"stream1.sim", "stream2.sim"};
    for(auto& f : files) {
        std::ofstream out(f);
        out << "SIM_IMG 10 10 [STREAM]";
        out.close();
    }

    bool success = true;
    for(auto& f : files) {
        if(!mgr.loadImage(f)) success = false;
    }

    for(auto& f : files) std::filesystem::remove(f);

    if (success) {
        std::cout << "v2 SUCCESS: Multiple streams loaded." << std::endl;
        return 0;
    }
    return 1;
}
