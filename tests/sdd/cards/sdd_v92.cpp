#include <iostream>
#include <fstream>
#include <filesystem>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    std::string test_file = "bomb.sim";
    std::ofstream out(test_file);
    out << "SIM_IMG 99999 99999 [BOMB]"; // Extremely large dimensions
    out.close();

    bool success = mgr.loadImage(test_file);
    std::filesystem::remove(test_file);

    if (!success) {
        std::cout << "v92 SUCCESS: Malformed header rejected." << std::endl;
        return 0;
    }
    std::cerr << "v92 FAILED: Malformed header accepted." << std::endl;
    return 1;
}
