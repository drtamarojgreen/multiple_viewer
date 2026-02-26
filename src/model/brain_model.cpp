#include "brain_model.h"
#include <cmath>

namespace model {

void BrainModel::addRegion(const BrainRegion& region) {
    regions_[region.id] = region;
}

void BrainModel::addPathway(const BrainPathway& pathway) {
    pathways_[pathway.id] = pathway;
}

const BrainRegion* BrainModel::getRegion(const RegionID& id) const {
    auto it = regions_.find(id);
    return (it != regions_.end()) ? &it->second : nullptr;
}

const BrainPathway* BrainModel::getPathway(const PathwayID& id) const {
    auto it = pathways_.find(id);
    return (it != pathways_.end()) ? &it->second : nullptr;
}

RegionID BrainModel::findRegionAt(const Vec3& point) const {
    // Simple exhaustive search using bounding spheres for now.
    // To be optimized with spatial indexing (Octree) in Phase 2.
    for (const auto& pair : regions_) {
        const auto& region = pair.second;
        float dx = point.x - region.center.x;
        float dy = point.y - region.center.y;
        float dz = point.z - region.center.z;
        float distSq = dx*dx + dy*dy + dz*dz;
        if (distSq <= region.radius * region.radius) {
            return region.id;
        }
    }
    return "";
}

void BrainModel::clear() {
    regions_.clear();
    pathways_.clear();
}

} // namespace model
