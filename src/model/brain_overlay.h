#ifndef BRAIN_OVERLAY_H
#define BRAIN_OVERLAY_H

#include "model_common.h"
#include <unordered_map>
#include <vector>

namespace model {

struct OverlayMapping {
    int graphNodeId;
    RegionID regionId;
    PathwayID pathwayId; // Optional, can be both or one
};

class BrainOverlay {
public:
    BrainOverlay() = default;

    void addMapping(const OverlayMapping& mapping);
    
    RegionID getRegionForNode(int nodeId) const;
    PathwayID getPathwayForNode(int nodeId) const;
    
    std::vector<int> getNodesInRegion(const RegionID& regionId) const;

    const std::unordered_map<int, OverlayMapping>& getAllMappings() const { return mappings_; }

private:
    std::unordered_map<int, OverlayMapping> mappings_;
};

} // namespace model

#endif // BRAIN_OVERLAY_H
