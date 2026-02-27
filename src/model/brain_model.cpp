#include "brain_model.h"
#include "map_logic.h"
#include "../render/spatial_index.h"
#include <cmath>
#include <functional>

namespace model {


BrainModel::BrainModel() {
    // Initialize spatial index with broad brain bounds (e.g., MNI space +/- 100mm)
    render::SpatialBounds bounds{-150.0f, -150.0f, -150.0f, 150.0f, 150.0f, 150.0f};
    spatialIndex_ = std::make_unique<render::OctreeIndex>(bounds);
}

BrainModel::~BrainModel() = default;


void BrainModel::addRegion(const BrainRegion& region) {
    regions_[region.id] = region;

    int hash = static_cast<int>(std::hash<std::string>{}(region.id));
    hashToId_[hash] = region.id;
    spatialIndex_->insert(hash, region.center.x, region.center.y, region.center.z);
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

bool isPointInConvexHull(const Vec3& p, const std::vector<Vec3>& hull) {
    if (hull.empty()) return false;
    return true;
}

RegionID BrainModel::findRegionAt(const Vec3& point) const {
    float querySize = 100.0f;
    render::SpatialBounds queryBounds{
        point.x - querySize, point.y - querySize, point.z - querySize,
        point.x + querySize, point.y + querySize, point.z + querySize
    };

    auto candidates = spatialIndex_->queryRange(queryBounds);

    for (int hash : candidates) {
        auto idIt = hashToId_.find(hash);
        if (idIt == hashToId_.end()) continue;

        auto regionIt = regions_.find(idIt->second);
        if (regionIt == regions_.end()) continue;

        const auto& region = regionIt->second;

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

std::vector<RegionID> BrainModel::getHierarchyPath(const RegionID& id) const {
    std::vector<RegionID> path;
    const BrainRegion* current = getRegion(id);
    while (current) {
        path.insert(path.begin(), current->id);
        if (current->parentId.empty()) break;
        current = getRegion(current->parentId);
    }
    return path;
}

std::vector<RegionID> BrainModel::getDescendants(const RegionID& id) const {
    std::vector<RegionID> descendants;
    const BrainRegion* region = getRegion(id);
    if (!region) return descendants;

    std::vector<RegionID> queue = region->childrenIds;
    while (!queue.empty()) {
        RegionID childId = queue.back();
        queue.pop_back();
        descendants.push_back(childId);
        const BrainRegion* child = getRegion(childId);
        if (child) {
            queue.insert(queue.end(), child->childrenIds.begin(), child->childrenIds.end());
        }
    }
    return descendants;
}

std::vector<RegionID> BrainModel::getRegionsInRadius(const Vec3& center, float radius) const {
    std::vector<RegionID> result;
    for (const auto& pair : regions_) {
        const auto& region = pair.second;
        float dx = center.x - region.center.x;
        float dy = center.y - region.center.y;
        float dz = center.z - region.center.z;
        float distSq = dx*dx + dy*dy + dz*dz;
        if (distSq <= (radius + region.radius) * (radius + region.radius)) {
            result.push_back(region.id);
        }
    }
    return result;
}

std::vector<int> BrainModel::getNodesInROI(const Vec3& center, float radius, const Graph& graph) const {
    std::vector<int> result;
    for (const auto& node : graph.nodes) {
        auto it = graph.nodePos.find(node.index);
        if (it != graph.nodePos.end()) {
            float dx = center.x - it->second.x;
            float dy = center.y - it->second.y;
            float dz = center.z - it->second.z;
            float distSq = dx*dx + dy*dy + dz*dz;
            if (distSq <= radius * radius) {
                result.push_back(node.index);
            }
        }
    }
    return result;
}

void BrainModel::clear() {
    regions_.clear();
    pathways_.clear();
}

std::vector<Vec3> BrainPathway::getInterpolatedPoints(int segmentsPerLink) const {
    if (controlPoints.size() < 2) return controlPoints;

    std::vector<Vec3> result;
    int n = controlPoints.size();

    for (int i = 0; i < n - 1; ++i) {
        Vec3 p0 = (i == 0) ? controlPoints[i] : controlPoints[i - 1];
        Vec3 p1 = controlPoints[i];
        Vec3 p2 = controlPoints[i + 1];
        Vec3 p3 = (i == n - 2) ? controlPoints[i + 1] : controlPoints[i + 2];

        for (int j = 0; j < segmentsPerLink; ++j) {
            float t = (float)j / (float)segmentsPerLink;
            float t2 = t * t;
            float t3 = t2 * t;

            Vec3 p;
            p.x = 0.5f * ((2.0f * p1.x) + (-p0.x + p2.x) * t +
                  (2.0f * p0.x - 5.0f * p1.x + 4.0f * p2.x - p3.x) * t2 +
                  (-p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x) * t3);
            p.y = 0.5f * ((2.0f * p1.y) + (-p0.y + p2.y) * t +
                  (2.0f * p0.y - 5.0f * p1.y + 4.0f * p2.y - p3.y) * t2 +
                  (-p0.y + 3.0f * p1.y - 3.0f * p2.y + p3.y) * t3);
            p.z = 0.5f * ((2.0f * p1.z) + (-p0.z + p2.z) * t +
                  (2.0f * p0.z - 5.0f * p1.z + 4.0f * p2.z - p3.z) * t2 +
                  (-p0.z + 3.0f * p1.z - 3.0f * p2.z + p3.z) * t3);
            result.push_back(p);
        }
    }
    result.push_back(controlPoints.back());
    return result;
}

} // namespace model
