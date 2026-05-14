#include <iostream>
#include "render/image_renderer.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    render::ImageRenderer renderer;
    renderer.initialize();
    if (!renderer.setZoom(-1.0f)) {
        std::cout << "v12 SUCCESS: Negative zoom rejected." << std::endl;
        return 0;
    }
    return 1;
}
