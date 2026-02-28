#ifndef MODEL_COMMON_H
#define MODEL_COMMON_H

#include <string>
#include <vector>
#include <set>

namespace model {

struct Vec3 {
    float x, y, z;
    
    bool operator==(const Vec3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

using RegionID = std::string;
using PathwayID = std::string;

enum class Hemisphere {
    Left,
    Right,
    Midline,
    Bilateral,
    Unknown
};

enum class Lobe {
    Frontal,
    Parietal,
    Temporal,
    Occipital,
    Limbic,
    Insular,
    Subcortical,
    Cerebellum,
    Brainstem,
    Unknown
};

enum class Layer {
    Cortex,
    Subcortex,
    Brainstem,
    Cerebellum,
    Unknown
};

enum class PathwayType {
    Association,
    Commissural,
    Projection,
    Unknown
};

enum class Laterality {
    Ipsilateral,
    Contralateral,
    Bilateral,
    Unknown
};

struct TopographicCoordinate {
    int xIndex;
    int yIndex;
    int zIndex;

    bool operator==(const TopographicCoordinate& other) const {
        return xIndex == other.xIndex && yIndex == other.yIndex && zIndex == other.zIndex;
    }
};

struct PointProvenance {
    float confidence;
    std::string source;
    std::string timestamp;
};

enum class RelationType {
    Contains,
    Overlaps,
    Intersects,
    Adjacent,
    None
};

} // namespace model

#endif // MODEL_COMMON_H
