#ifndef BRAIN_TEXT_TOPOLOGY_H
#define BRAIN_TEXT_TOPOLOGY_H

#include "model_common.h"
#include "text_topology.h"
#include "brain_model.h"
#include <memory>

namespace model {

class LabelPlacementPolicy {
public:
    virtual ~LabelPlacementPolicy() = default;
    virtual void placeLabels(const BrainModel& model, TextCanvas& canvas) = 0;
};

class BrainTextTopology {
public:
    virtual ~BrainTextTopology() = default;

    virtual TextSlice generateSlice(const BrainModel& model, TextSlice::Plane plane, float position, int resolution) = 0;
    virtual std::vector<RegionID> getNeighborhood(const BrainModel& model, const RegionID& id) = 0;
    
    virtual std::string generateHierarchyTree(const BrainModel& model) = 0;
    virtual std::string generatePathwaySummary(const BrainModel& model) = 0;

    // Export methods (Requirements 58, 59, 60)
    virtual std::string exportToMarkdown(const TextSlice& slice) = 0;
    virtual std::string exportToJSON(const TextSlice& slice) = 0;
    virtual std::string exportToPlainText(const TextSlice& slice) = 0;
};


class TopologyIndexer {
public:
    virtual ~TopologyIndexer() = default;
    virtual void buildIndex(const BrainModel& model) = 0;
    virtual RegionID findNearestRegion(const Vec3& point) const = 0;
};

std::unique_ptr<TopologyIndexer> createTopologyIndexer();
std::unique_ptr<BrainTextTopology> createBrainTextTopology();

} // namespace model


#endif // BRAIN_TEXT_TOPOLOGY_H
