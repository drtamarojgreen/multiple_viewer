#ifndef SPATIAL_INDEX_H
#define SPATIAL_INDEX_H

#include <vector>
#include <memory>

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
    OctreeIndex(SpatialBounds bounds, int capacity = 8, int depth = 0)
        : bounds_(bounds), capacity_(capacity), depth_(depth), subdivided_(false) {}

    void insert(int nodeId, float x, float y, float z) override;
    std::vector<int> queryRange(const SpatialBounds& bounds) override;

private:
    static constexpr int MAX_DEPTH = 16;
    void subdivide();
    bool contains(const SpatialBounds& b, float x, float y, float z) const;
    bool intersects(const SpatialBounds& a, const SpatialBounds& b) const;

    struct Entry {
        int nodeId;
        float x, y, z;
    };

    SpatialBounds bounds_;
    int capacity_;
    int depth_;
    std::vector<Entry> entries_;
    std::unique_ptr<OctreeIndex> children_[8];
    bool subdivided_;
};



} // namespace render

#endif // SPATIAL_INDEX_H
