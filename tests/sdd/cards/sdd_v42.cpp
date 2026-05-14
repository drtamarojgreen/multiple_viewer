#include <iostream>
#include <fstream>
#include <filesystem>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    std::string utf8_file = "test_你好.sim";
    std::ofstream out(utf8_file);
    out << "SIM_IMG 10 10";
    out.close();
    bool success = mgr.loadImage(utf8_file);
    std::filesystem::remove(utf8_file);
    if (success) {
        std::cout << "v42 SUCCESS: UTF-8 filename verified." << std::endl;
        return 0;
    }
    return 1;
}
