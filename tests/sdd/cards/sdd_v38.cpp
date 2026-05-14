#include <iostream>
#include "render/image_renderer.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    render::ImageRenderer renderer;
    renderer.initialize();
    if (renderer.checkLayout("v38")) {
        std::cout << "v38 SUCCESS: Minimize/Restore verified." << std::endl;
        return 0;
    }
    return 1;
}
