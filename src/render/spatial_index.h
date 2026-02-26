#ifndef SPATIAL_INDEX_H
#define SPATIAL_INDEX_H

#include "../map_logic.h"
#include <vector>

namespace render {

struct SpatialBounds {
    float x1, y1, z1;
    float x2, y2, z2;
};

class SpatialIndex {
public:
    virtual ~SpatialIndex() = default;
    virtual void insert(int nodeId, float x, float y, float z) = 0;
    virtual std::vector<int> queryRange(const SpatialBounds& bounds) = 0;
};

class OctreeIndex : public SpatialIndex {
public:
    void insert(int nodeId, float x, float y, float z) override;
    std::vector<int> queryRange(const SpatialBounds& bounds) override;
};

} // namespace render

#endif // SPATIAL_INDEX_H
