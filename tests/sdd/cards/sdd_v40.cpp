#include <iostream>
#include "render/image_renderer.h"
#include "cpp/util/fact_utils.h"

int main() {
    auto facts = Sorrel::Sdd::Util::FactReader::readFacts("image_env.facts");
    render::ImageRenderer renderer;
    renderer.initialize();
    if (renderer.checkLayout("v40")) {
        std::cout << "v40 SUCCESS: Theme switching verified." << std::endl;
        return 0;
    }
    return 1;
}
