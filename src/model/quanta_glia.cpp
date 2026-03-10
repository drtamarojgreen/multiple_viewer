#include "quanta_glia.h"

namespace model {

std::shared_ptr<ModelRepository> QuantaGlia::getRepository(const std::string& id) {
    auto it = repositories_.find(id);
    if (it != repositories_.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<ModelRepository> QuantaGlia::createRepository(const std::string& id) {
    if (hasRepository(id)) {
        return getRepository(id);
    }
    auto repo = std::make_shared<ModelRepository>();
    repositories_[id] = repo;
    return repo;
}

bool QuantaGlia::hasRepository(const std::string& id) const {
    return repositories_.find(id) != repositories_.end();
}

void QuantaGlia::removeRepository(const std::string& id) {
    repositories_.erase(id);
}

std::vector<std::string> QuantaGlia::getAllRepositoryIds() const {
    std::vector<std::string> ids;
    for (const auto& pair : repositories_) {
        ids.push_back(pair.first);
    }
    return ids;
}

void QuantaGlia::clear() {
    repositories_.clear();
}

} // namespace model
