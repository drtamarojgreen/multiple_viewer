#ifndef BRAIN_MODEL_REGISTRY_H
#define BRAIN_MODEL_REGISTRY_H

#include "brain_model_manifest.h"
#include "brain_model.h"
#include <unordered_map>
#include <memory>
#include <string>

namespace model {

class BrainModelRegistry {
public:
    static BrainModelRegistry& getInstance() {
        static BrainModelRegistry instance;
        return instance;
    }

    void registerModel(const BrainModelManifest& manifest, std::unique_ptr<BrainModel> model);
    void unregisterModel(const std::string& id);

    BrainModel* getModel(const std::string& id);
    const BrainModelManifest* getManifest(const std::string& id) const;

    // Capability negotiation
    bool modelSupports(const std::string& id, const std::string& capability) const;

    // Dependency management
    std::vector<std::string> getDependencies(const std::string& id) const;
    bool checkIntegrity(const std::string& id) const;

    std::vector<std::string> getAllModelIds() const;

    void clear();

private:
    BrainModelRegistry() = default;
    ~BrainModelRegistry() = default;

    std::unordered_map<std::string, std::unique_ptr<BrainModel>> models_;
    std::unordered_map<std::string, BrainModelManifest> manifests_;
};

} // namespace model

#endif // BRAIN_MODEL_REGISTRY_H
