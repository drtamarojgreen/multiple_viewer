#ifndef BRAIN_CANONICAL_H
#define BRAIN_CANONICAL_H

#include "model_common.h"
#include <string>
#include <map>
#include <variant>
#include <vector>

namespace model {

using BrainAttributeValue = std::variant<double, int, std::string, bool, std::vector<float>>;

struct BrainNode {
    std::string id;
    std::string name;
    Vec3 coordinate;
    std::string type;
    std::map<std::string, BrainAttributeValue> attributes;
};

struct BrainEdge {
    std::string id;
    std::string sourceId;
    std::string targetId;
    float weight = 1.0f;
    float confidence = 1.0f;
    std::string modality;
    std::map<std::string, BrainAttributeValue> attributes;
};

} // namespace model

#endif // BRAIN_CANONICAL_H
