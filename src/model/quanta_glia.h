#ifndef QUANTA_GLIA_H
#define QUANTA_GLIA_H

#include "model_repository.h"
#include <map>
#include <string>
#include <memory>
#include <vector>

namespace model {

class QuantaGlia {
public:
    static QuantaGlia& getInstance() {
        static QuantaGlia instance;
        return instance;
    }

    std::shared_ptr<ModelRepository> getRepository(const std::string& id);
    std::shared_ptr<ModelRepository> createRepository(const std::string& id);
    bool hasRepository(const std::string& id) const;
    void removeRepository(const std::string& id);
    std::vector<std::string> getAllRepositoryIds() const;

    void clear();

private:
    QuantaGlia() = default;
    ~QuantaGlia() = default;

    QuantaGlia(const QuantaGlia&) = delete;
    QuantaGlia& operator=(const QuantaGlia&) = delete;

    std::map<std::string, std::shared_ptr<ModelRepository>> repositories_;
};

} // namespace model

#endif // QUANTA_GLIA_H
