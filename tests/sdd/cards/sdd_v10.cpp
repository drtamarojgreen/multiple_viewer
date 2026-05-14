#include <iostream>
#include "render/image_renderer.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    render::ImageRenderer renderer;
    renderer.initialize();
    if (renderer.toggleFullscreen() && renderer.checkLayout("v10")) {
        std::cout << "v10 SUCCESS: Fullscreen scaling verified." << std::endl;
        return 0;
    }
    return 1;
}
