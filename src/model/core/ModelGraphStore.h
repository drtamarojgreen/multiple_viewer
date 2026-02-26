#pragma once

#include "model/core/contracts/IModelGraphStore.h"
#include <unordered_map>
#include <optional>
#include <vector>
#include <string>
#include <cstdint>

namespace brain_model::core {

class ModelGraphStore : public contracts::IModelGraphStore {
public:
    void add_node(const contracts::GraphNode& node) override;
    void add_edge(const contracts::GraphEdge& edge) override;
    std::optional<contracts::GraphNode> get_node(const std::string& id) const override;
    std::vector<contracts::GraphNode> find_nodes_by_domain(const std::string& domain) const override;
    uint32_t current_version() const override;
    void checkout_version(uint32_t version) override;

private:
    std::unordered_map<std::string, contracts::GraphNode> m_nodes;
    std::unordered_map<std::string, contracts::GraphEdge> m_edges;
    uint32_t m_version = 0;
};

} // namespace brain_model::core
