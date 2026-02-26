#include "brain_model_registry.h"
#include <algorithm>

namespace model {

void BrainModelRegistry::registerModel(const BrainModelManifest& manifest, std::unique_ptr<BrainModel> model) {
    std::string id = manifest.id;
    manifests_[id] = manifest;
    models_[id] = std::move(model);
}

void BrainModelRegistry::unregisterModel(const std::string& id) {
    manifests_.erase(id);
    models_.erase(id);
}

BrainModel* BrainModelRegistry::getModel(const std::string& id) {
    auto it = models_.find(id);
    if (it != models_.end()) {
        return it->second.get();
    }
    return nullptr;
}

const BrainModelManifest* BrainModelRegistry::getManifest(const std::string& id) const {
    auto it = manifests_.find(id);
    if (it != manifests_.end()) {
        return &it->second;
    }
    return nullptr;
}

std::vector<std::string> BrainModelRegistry::getAllModelIds() const {
    std::vector<std::string> ids;
    for (const auto& pair : manifests_) {
        ids.push_back(pair.first);
    }
    return ids;
}

void BrainModelRegistry::clear() {
    models_.clear();
    manifests_.clear();
}

} // namespace model
