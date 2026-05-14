#include <iostream>
#include <fstream>
#include <filesystem>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    std::ofstream out("v29.sim");
    out << "SIM_IMG 10 20";
    out.close();
    mgr.loadImage("v29.sim");
    io::ExifData exif; exif.orientation = "Rotate90";
    mgr.setExif(exif);
    mgr.autoCorrectOrientation();
    auto img = mgr.getActiveImage();
    std::filesystem::remove("v29.sim");
    if (img && img->width == 20 && img->height == 10) {
        std::cout << "v29 SUCCESS: EXIF auto-correction verified." << std::endl;
        return 0;
    }
    return 1;
}
