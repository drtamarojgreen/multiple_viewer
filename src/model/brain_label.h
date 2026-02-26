#ifndef BRAIN_LABEL_H
#define BRAIN_LABEL_H

#include "model_common.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace model {

struct BrainLabel {
    std::string id;
    std::string displayName;
    std::string description;
    std::string category; // e.g., "Anatomy", "Function", "Connectivity"
};

class BrainLabelSet {
public:
    BrainLabelSet() = default;

    void addLabel(const BrainLabel& label);
    const BrainLabel* getLabel(const std::string& id) const;
    
    std::vector<const BrainLabel*> getLabelsByCategory(const std::string& category) const;

    const std::unordered_map<std::string, BrainLabel>& getAllLabels() const { return labels_; }

private:
    std::unordered_map<std::string, BrainLabel> labels_;
};

} // namespace model

#endif // BRAIN_LABEL_H
