#ifndef MODEL_COMMON_H
#define MODEL_COMMON_H

#include <string>
#include <vector>

namespace model {

struct Vec3 {
    float x, y, z;
    
    bool operator==(const Vec3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

using RegionID = std::string;
using PathwayID = std::string;

} // namespace model

#endif // MODEL_COMMON_H
