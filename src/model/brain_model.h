#ifndef BRAIN_MODEL_H
#define BRAIN_MODEL_H

#include "model_common.h"
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <set>

namespace render {
    class OctreeIndex;
}

class Graph;

namespace model {


enum class ActivationLevel {
    Resting,
    MildlyActive,
    HighlyActive,
    Suppressed
};

enum class PathwayDirection {
    Unidirectional,
    Bidirectional,
    Inhibitory
};

struct BrainRegion {
    RegionID id;
    std::string name;
    Vec3 center;
    float radius; // Bounding sphere for fast rejection
    std::vector<Vec3> geometry; // Detailed vertex data
    std::vector<Vec3> convexHull; // Feature 2: Convex hull vertices

    // Metadata Expansion
    std::string regionCode;
    std::vector<std::string> aliases;
    Hemisphere hemisphere = Hemisphere::Unknown;
    Lobe lobe = Lobe::Unknown;
    Layer layer = Layer::Unknown;

    RegionID parentId; // Merged: using parentId (camelCase from my branch or main's parentID)
    std::vector<RegionID> childrenIds; // From my branch

    ActivationLevel activation = ActivationLevel::Resting; // Feature 12: Region Activation State

    float surfaceArea = 0.0f;
    float volume = 0.0f;
    int displayPriority = 0; // Feature extension for LOD/Filtering
};


struct BrainPathway {
    PathwayID id;
    std::string name;
    RegionID sourceRegion;
    RegionID targetRegion;
    std::vector<Vec3> controlPoints;

    PathwayDirection direction = PathwayDirection::Unidirectional;
    float strength = 1.0f;
    PathwayType type = PathwayType::Unknown;

    // Feature 6: Spline Interpolation
    std::vector<Vec3> getInterpolatedPoints(int segmentsPerLink = 10) const;
};


class BrainModel {
public:
    BrainModel();
    ~BrainModel();


    void addRegion(const BrainRegion& region);
    void addPathway(const BrainPathway& pathway);

    const BrainRegion* getRegion(const RegionID& id) const;
    const BrainPathway* getPathway(const PathwayID& id) const;

    RegionID findRegionAt(const Vec3& point) const;

    const std::unordered_map<RegionID, BrainRegion>& getRegions() const { return regions_; }
    const std::unordered_map<PathwayID, BrainPathway>& getPathways() const { return pathways_; }

    // Hierarchy traversal (from my branch)
    std::vector<RegionID> getHierarchyPath(const RegionID& id) const;
    std::vector<RegionID> getDescendants(const RegionID& id) const;

    // ROI support (from my branch)
    std::vector<RegionID> getRegionsInRadius(const Vec3& center, float radius) const;
    std::vector<int> getNodesInROI(const Vec3& center, float radius, const Graph& graph) const;

    // Feature 4: Atlas Version Control
    std::string versionTag;
    std::string timestamp;

    void clear();

private:
    std::unordered_map<RegionID, BrainRegion> regions_;
    std::unordered_map<PathwayID, BrainPathway> pathways_;
    std::unique_ptr<render::OctreeIndex> spatialIndex_; // Feature 1
    std::unordered_map<int, RegionID> hashToId_; // Reverse mapping for Octree candidates
};



} // namespace model

#endif // BRAIN_MODEL_H
