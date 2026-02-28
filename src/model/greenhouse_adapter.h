#ifndef GREENHOUSE_ADAPTER_H
#define GREENHOUSE_ADAPTER_H

#include "brain_canonical.h"
#include <string>
#include <map>

namespace model {

// Represents a JSON-like object from external JS environments
struct ExternalEntity {
    std::string uid;
    std::string label;
    std::map<std::string, std::string> properties;
    std::vector<float> position;
};

class GreenhouseCompatibilityAdapter {
public:
    static BrainNode toCanonicalNode(const ExternalEntity& entity) {
        BrainNode node;
        node.id = entity.uid;
        node.name = entity.label;
        if (entity.position.size() >= 3) {
            node.coordinate = { entity.position[0], entity.position[1], entity.position[2] };
        }
        for (const auto& [key, val] : entity.properties) {
            node.attributes[key] = val;
        }
        return node;
    }
};

} // namespace model

#endif // GREENHOUSE_ADAPTER_H
