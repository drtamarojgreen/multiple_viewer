#include "brain_overlay.h"

namespace model {

void BrainOverlay::addMapping(const OverlayMapping& mapping) {
    mappings_[mapping.graphNodeId] = mapping;
}

RegionID BrainOverlay::getRegionForNode(int nodeId) const {
    auto it = mappings_.find(nodeId);
    return (it != mappings_.end()) ? it->second.regionId : "";
}

PathwayID BrainOverlay::getPathwayForNode(int nodeId) const {
    auto it = mappings_.find(nodeId);
    return (it != mappings_.end()) ? it->second.pathwayId : "";
}

std::vector<int> BrainOverlay::getNodesInRegion(const RegionID& regionId) const {
    std::vector<int> results;
    for (const auto& pair : mappings_) {
        if (pair.second.regionId == regionId) {
            results.push_back(pair.first);
        }
    }
    return results;
}

} // namespace model
