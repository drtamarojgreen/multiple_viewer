#ifndef MODEL_REPOSITORY_H
#define MODEL_REPOSITORY_H

#include "brain_model.h"
#include "brain_label.h"
#include "brain_overlay.h"
#include "brain_text_topology.h"
#include "brain_text_topology.h"
#include <memory>
#include <string>

namespace model {

class ModelRepository {
public:
    static ModelRepository& getInstance() {
        static ModelRepository instance;
        return instance;
    }

    // Load and reload (Requirement 10)
    // Load and reload (Requirement 10)
    bool loadAtlas(const std::string& filepath);
    bool reloadAtlas();

    bool loadLabels(const std::string& filepath);
    bool loadOverlay(const std::string& filepath);

    // Export (Requirement 15)
    bool exportToJSON(const std::string& filepath);

    const BrainModel& getModel() const { return model_; }
    const BrainLabelSet& getLabels() const { return labels_; }
    const BrainOverlay& getOverlay() const { return overlay_; }

    // Topology Services
    BrainTextTopology& getTopology();
    TopologyIndexer& getIndexer();

    // Mapping (Requirement 14)
    int getSubjectIndexForRegion(const RegionID& id) const;
    void setSubjectIndexForRegion(const RegionID& id, int subjectIndex);

    // Mirroring (Requirement 13)
    void mirrorAtlas();

    void clearAll();


private:
    ModelRepository();
    ~ModelRepository() = default;
    
    std::string currentAtlasPath_;


    // Explicitly non-copyable
    ModelRepository(const ModelRepository&) = delete;
    ModelRepository& operator=(const ModelRepository&) = delete;

    BrainModel model_;
    std::unordered_map<RegionID, int> regionToSubjectMap_;



    std::unordered_map<RegionID, int> regionToSubjectMap_;



    BrainLabelSet labels_;
    BrainOverlay overlay_;

    std::unique_ptr<BrainTextTopology> topology_;
    std::unique_ptr<TopologyIndexer> indexer_;
};


} // namespace model

#endif // MODEL_REPOSITORY_H
