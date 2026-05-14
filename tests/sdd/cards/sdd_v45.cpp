#include <iostream>
#include <fstream>
#include <filesystem>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;

    // Attempting to load a sensitive system path
    bool success = mgr.loadImage("/etc/passwd");

    if (!success) {
        std::cout << "v45 SUCCESS: Path traversal rejected." << std::endl;
        return 0;
    }
    std::cerr << "v45 FAILED: Path traversal accepted." << std::endl;
    return 1;
}
