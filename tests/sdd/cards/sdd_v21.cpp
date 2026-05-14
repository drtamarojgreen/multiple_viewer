#include <iostream>
#include "render/image_renderer.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    render::ImageRenderer renderer;
    renderer.initialize();
    if (renderer.checkAnalytics("v21")) {
        std::cout << "v21 SUCCESS: Mono histogram verified." << std::endl;
        return 0;
    }
    return 1;
}
