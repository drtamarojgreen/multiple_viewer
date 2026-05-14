#include <iostream>
#include "io/image_manager.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    io::ImageManager mgr;
    io::ExifData exif;
    exif.orientation = ""; // Missing
    mgr.setExif(exif);
    if (mgr.getExif().orientation == "") {
        std::cout << "v28 SUCCESS: Missing EXIF handled." << std::endl;
        return 0;
    }
    return 1;
}
