#include <iostream>
#include "render/image_renderer.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    render::ImageRenderer renderer;
    renderer.initialize();
    renderer.setZoom(2.0f);
    if (renderer.setZoom(1.0f)) {
        std::cout << "v13 SUCCESS: Zoom reset verified." << std::endl;
        return 0;
    }
    return 1;
}
