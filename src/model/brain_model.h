#ifndef BRAIN_MODEL_H
#define BRAIN_MODEL_H

#include "model_common.h"
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

namespace model {

struct BrainRegion {
    RegionID id;
    std::string name;
    Vec3 center;
    float radius; // Simple bounding sphere for now
    std::vector<Vec3> geometry; // Voxel or vertex data
};

struct BrainPathway {
    PathwayID id;
    std::string name;
    RegionID sourceRegion;
    RegionID targetRegion;
    std::vector<Vec3> controlPoints; // Spline or path points
};

class BrainModel {
public:
    BrainModel() = default;
    ~BrainModel() = default;

    void addRegion(const BrainRegion& region);
    void addPathway(const BrainPathway& pathway);

    const BrainRegion* getRegion(const RegionID& id) const;
    const BrainPathway* getPathway(const PathwayID& id) const;

    // Spatial lookup: Find the region containing this point
    RegionID findRegionAt(const Vec3& point) const;

    const std::unordered_map<RegionID, BrainRegion>& getRegions() const { return regions_; }
    const std::unordered_map<PathwayID, BrainPathway>& getPathways() const { return pathways_; }

    void clear();

private:
    std::unordered_map<RegionID, BrainRegion> regions_;
    std::unordered_map<PathwayID, BrainPathway> pathways_;
};

} // namespace model

#endif // BRAIN_MODEL_H
