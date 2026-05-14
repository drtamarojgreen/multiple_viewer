#include <iostream>
#include <fstream>
#include <filesystem>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    std::string test_file = "corrupt.sim";
    std::ofstream out(test_file);
    out << "NOT_SIM_FORMAT 0 0";
    out.close();

    bool success = mgr.loadImage(test_file);
    std::filesystem::remove(test_file);

    if (!success) {
        std::cout << "v4 SUCCESS: Corrupted file rejected." << std::endl;
        return 0;
    }
    std::cerr << "v4 FAILED: Corrupted file accepted." << std::endl;
    return 1;
}
