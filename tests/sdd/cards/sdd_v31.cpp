#include <iostream>
#include <vector>
#include <filesystem>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    std::vector<std::filesystem::path> paths = {"not_image.txt"};
    if (!mgr.handleDrop(paths)) {
        std::cout << "v31 SUCCESS: Non-image drop rejected." << std::endl;
        return 0;
    }
    return 1;
}
