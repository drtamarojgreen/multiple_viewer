#include "visual_mapper.h"
#include "../map_logic.h"

namespace render {

std::string VisualMapper::getColorForWeight(int weight) {
    if (weight > Config::nodeWeightThresholdHigh) return "RED";
    if (weight > Config::nodeWeightThresholdLow) return "YELLOW";
    return "WHITE";
}

int VisualMapper::getSizeForDegree(int degree) {
    return 1 + (degree / 5);
}

} // namespace render
