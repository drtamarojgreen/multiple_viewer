#ifndef MODEL_REPOSITORY_H
#define MODEL_REPOSITORY_H

#include "brain_model.h"
#include "brain_label.h"
#include "brain_overlay.h"
#include <memory>
#include <string>

namespace model {

class ModelRepository {
public:
    static ModelRepository& getInstance() {
        static ModelRepository instance;
        return instance;
    }

    // Load from specific textual assets
    bool loadAtlas(const std::string& filepath);
    bool loadLabels(const std::string& filepath);
    bool loadOverlay(const std::string& filepath);

    const BrainModel& getModel() const { return model_; }
    const BrainLabelSet& getLabels() const { return labels_; }
    const BrainOverlay& getOverlay() const { return overlay_; }

    void clearAll();

private:
    ModelRepository() = default;
    ~ModelRepository() = default;
    
    // Explicitly non-copyable
    ModelRepository(const ModelRepository&) = delete;
    ModelRepository& operator=(const ModelRepository&) = delete;

    BrainModel model_;
    BrainLabelSet labels_;
    BrainOverlay overlay_;
};

} // namespace model

#endif // MODEL_REPOSITORY_H
