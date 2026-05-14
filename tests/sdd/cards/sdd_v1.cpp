#include <iostream>
#include <fstream>
#include <filesystem>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    if (facts.empty()) return 1;

    io::ImageManager mgr;
    std::string test_file = "test_v1.sim";
    std::ofstream out(test_file);
    out << "SIM_IMG 100 100 [V1]";
    out.close();

    bool success = mgr.loadImage(test_file);
    std::filesystem::remove(test_file);

    if (success && mgr.getActiveImage() && mgr.getActiveImage()->width == 100) {
        std::cout << "v1 SUCCESS" << std::endl;
        return 0;
    }
    std::cerr << "v1 FAILED" << std::endl;
    return 1;
}
