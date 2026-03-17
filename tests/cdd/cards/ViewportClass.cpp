#include <iostream>
#include <map>
#include "viewer_logic.h"
#include "../cpp/util/fact_utils.h"

using namespace Chai::Cdd::Util;

// @Card: viewport_zoom_in_verification
// @Results viewport_zoom_in_operational == true
void viewport_zoom_in_card(const std::map<std::string, std::string>& facts) {
    ViewContext view;
    ZoomLevel initial = view.zoomLevel;
    view.zoomIn();
    bool operational = (view.zoomLevel > initial);
    std::cout << "viewport_zoom_in_operational = " << (operational ? "true" : "false") << std::endl;
}

// @Card: viewport_pan_verification
// @Results viewport_pan_operational == true
void viewport_pan_card(const std::map<std::string, std::string>& facts) {
    ViewContext view;
    int initialX = view.panX;
    view.pan(10, 0);
    bool operational = (view.panX == initialX + 10);
    std::cout << "viewport_pan_operational = " << (operational ? "true" : "false") << std::endl;
}

int main(int argc, char* argv[]) {
    auto facts = FactReader::readFacts("tests/cdd/facts/viewport.facts");
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "pan") viewport_pan_card(facts);
        else viewport_zoom_in_card(facts);
    } else {
        viewport_zoom_in_card(facts);
    }
    return 0;
}
