#include <iostream>
#include <filesystem>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    std::filesystem::create_directory("unicode_目录");
    bool success = mgr.loadDirectory("unicode_目录");
    std::filesystem::remove("unicode_目录");
    if (success) {
        std::cout << "v43 SUCCESS: Unicode directory traversal verified." << std::endl;
        return 0;
    }
    return 1;
}
