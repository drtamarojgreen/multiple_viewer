#include "visual_mapper.h"

namespace render {

std::string VisualMapper::getColorForWeight(int weight) {
    if (weight > 10) return "RED";
    if (weight > 5) return "YELLOW";
    return "WHITE";
}

int VisualMapper::getSizeForDegree(int degree) {
    return 1 + (degree / 5);
}

} // namespace render
