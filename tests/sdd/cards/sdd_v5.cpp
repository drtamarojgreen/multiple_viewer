#include <iostream>
#include <fstream>
#include <filesystem>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    std::ofstream out("test.txt");
    out << "not an image";
    out.close();

    bool success = mgr.loadImage("test.txt");
    std::filesystem::remove("test.txt");

    if (!success) {
        std::cout << "v5 SUCCESS: Unsupported extension ignored." << std::endl;
        return 0;
    }
    return 1;
}
