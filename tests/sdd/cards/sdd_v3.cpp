#include <iostream>
#include <filesystem>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    std::filesystem::create_directory("empty_dir");

    bool success = mgr.loadDirectory("empty_dir");
    std::filesystem::remove("empty_dir");

    if (success) {
        std::cout << "v3 SUCCESS: Empty directory handled." << std::endl;
        return 0;
    }
    return 1;
}
