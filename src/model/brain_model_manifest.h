#ifndef BRAIN_MODEL_MANIFEST_H
#define BRAIN_MODEL_MANIFEST_H

#include <string>
#include <vector>
#include <map>

namespace model {

enum class ModelFamily {
    STRUCTURAL,
    CONNECTIVITY,
    TEMPORAL,
    SIMULATION,
    HYBRID
};

struct ModelCapabilities {
    bool supportsTemporal = false;
    bool supportsLayers = false;
    bool supportsEdgeWeights = false;
    bool supportsProbabilistic = false;
};

struct BrainModelManifest {
    std::string id;
    std::string name;
    std::string version;
    ModelFamily family = ModelFamily::STRUCTURAL;
    ModelCapabilities capabilities;
    std::string provenance;
    std::vector<std::string> dependencies;
    std::map<std::string, std::string> metadata;
};

} // namespace model

#endif // BRAIN_MODEL_MANIFEST_H
