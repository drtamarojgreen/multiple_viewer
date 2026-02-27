#ifndef BRAIN_CANONICAL_H
#define BRAIN_CANONICAL_H

#include "model_common.h"
#include <string>
#include <map>
#include <variant>
#include <vector>

namespace model {

struct TimeSeries {
    std::vector<float> samples;
    float sampleRateHz = 1.0f;
    std::string unit;
};

using BrainAttributeValue = std::variant<double, int, std::string, bool, std::vector<float>, TimeSeries>;

struct ProbabilisticMembership {
    std::string regionId;
    float probability;
};

struct BrainNode {
    std::string id;
    std::string name;
    Vec3 coordinate;
    std::string type;
    std::map<std::string, BrainAttributeValue> attributes;

    // Probabilistic region membership
    std::vector<ProbabilisticMembership> memberships;
};

enum class EdgeModality {
    STRUCTURAL,
    FUNCTIONAL,
    EFFECTIVE,
    VIRTUAL
};

struct BrainEdge {
    std::string id;
    std::string sourceId;
    std::string targetId;
    float weight = 1.0f;
    float confidence = 1.0f;
    float delay_ms = 0.0f;

    // Confidence intervals
    float lowerBound = 1.0f;
    float upperBound = 1.0f;

    EdgeModality modality = EdgeModality::STRUCTURAL;
    std::map<std::string, BrainAttributeValue> attributes;
};

} // namespace model

#endif // BRAIN_CANONICAL_H
