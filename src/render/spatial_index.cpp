#include "spatial_index.h"

namespace render {

void OctreeIndex::insert(int nodeId, float x, float y, float z) {
    if (!contains(bounds_, x, y, z)) return;

    if (subdivided_) {
        for (int i = 0; i < 8; ++i) {
            children_[i]->insert(nodeId, x, y, z);
        }
        return;
    }

    if (entries_.size() < (size_t)capacity_ || depth_ >= MAX_DEPTH) {
        entries_.push_back({nodeId, x, y, z});
        return;
    }

    subdivide();

    // After subdivision, this node is empty and subdivided_ is true
    // Need to re-insert the current node as well
    insert(nodeId, x, y, z);
}


std::vector<int> OctreeIndex::queryRange(const SpatialBounds& bounds) {
    std::vector<int> found;
    if (!intersects(bounds_, bounds)) return found;

    for (const auto& entry : entries_) {
        if (contains(bounds, entry.x, entry.y, entry.z)) {
            found.push_back(entry.nodeId);
        }
    }

    if (subdivided_) {
        for (int i = 0; i < 8; ++i) {
            auto childFound = children_[i]->queryRange(bounds);
            found.insert(found.end(), childFound.begin(), childFound.end());
        }
    }

    return found;
}

void OctreeIndex::subdivide() {
    float midX = (bounds_.x1 + bounds_.x2) / 2.0f;
    float midY = (bounds_.y1 + bounds_.y2) / 2.0f;
    float midZ = (bounds_.z1 + bounds_.z2) / 2.0f;

    int nextDepth = depth_ + 1;
    children_[0] = std::make_unique<OctreeIndex>(SpatialBounds{bounds_.x1, bounds_.y1, bounds_.z1, midX, midY, midZ}, capacity_, nextDepth);
    children_[1] = std::make_unique<OctreeIndex>(SpatialBounds{midX, bounds_.y1, bounds_.z1, bounds_.x2, midY, midZ}, capacity_, nextDepth);
    children_[2] = std::make_unique<OctreeIndex>(SpatialBounds{bounds_.x1, midY, bounds_.z1, midX, bounds_.y2, midZ}, capacity_, nextDepth);
    children_[3] = std::make_unique<OctreeIndex>(SpatialBounds{midX, midY, bounds_.z1, bounds_.x2, bounds_.y2, midZ}, capacity_, nextDepth);
    children_[4] = std::make_unique<OctreeIndex>(SpatialBounds{bounds_.x1, bounds_.y1, midZ, midX, midY, bounds_.z2}, capacity_, nextDepth);
    children_[5] = std::make_unique<OctreeIndex>(SpatialBounds{midX, bounds_.y1, midZ, bounds_.x2, midY, bounds_.z2}, capacity_, nextDepth);
    children_[6] = std::make_unique<OctreeIndex>(SpatialBounds{bounds_.x1, midY, midZ, midX, bounds_.y2, bounds_.z2}, capacity_, nextDepth);
    children_[7] = std::make_unique<OctreeIndex>(SpatialBounds{midX, midY, midZ, bounds_.x2, bounds_.y2, bounds_.z2}, capacity_, nextDepth);

    subdivided_ = true;

    // Move existing entries to children
    for (const auto& entry : entries_) {
        for (int i = 0; i < 8; ++i) {
            children_[i]->insert(entry.nodeId, entry.x, entry.y, entry.z);
        }
    }
    entries_.clear();
}


bool OctreeIndex::contains(const SpatialBounds& b, float x, float y, float z) const {
    return x >= b.x1 && x <= b.x2 && y >= b.y1 && y <= b.y2 && z >= b.z1 && z <= b.z2;
}

bool OctreeIndex::intersects(const SpatialBounds& a, const SpatialBounds& b) const {
    return (a.x1 <= b.x2 && a.x2 >= b.x1) &&
           (a.y1 <= b.y2 && a.y2 >= b.y1) &&
           (a.z1 <= b.z2 && a.z2 >= b.z1);
}


} // namespace render
