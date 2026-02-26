#include "brain_label.h"

namespace model {

void BrainLabelSet::addLabel(const BrainLabel& label) {
    labels_[label.id] = label;
}

const BrainLabel* BrainLabelSet::getLabel(const std::string& id) const {
    auto it = labels_.find(id);
    return (it != labels_.end()) ? &it->second : nullptr;
}

std::vector<const BrainLabel*> BrainLabelSet::getLabelsByCategory(const std::string& category) const {
    std::vector<const BrainLabel*> results;
    for (const auto& pair : labels_) {
        if (pair.second.category == category) {
            results.push_back(&pair.second);
        }
    }
    return results;
}

} // namespace model
