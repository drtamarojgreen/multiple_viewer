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

struct ModelAsset {
    std::string id;
    std::string path;
    std::string type;
    std::string checksum;
};

struct BrainModelManifest {
    std::string id;
    std::string name;
    std::string version;
    ModelFamily family = ModelFamily::STRUCTURAL;
    ModelCapabilities capabilities;
    std::string provenance;
    std::vector<std::string> dependencies;
    std::vector<ModelAsset> assets;
    std::map<std::string, std::string> metadata;
    std::string schemaVersion = "1.0";
};

} // namespace model

#endif // BRAIN_MODEL_MANIFEST_H
