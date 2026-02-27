#include "model/core/ModelGraphStore.h"

namespace brain_model::core {

void ModelGraphStore::add_node(const contracts::GraphNode& node) {
    m_nodes[node.id] = node;
    m_version++;
}

void ModelGraphStore::add_edge(const contracts::GraphEdge& edge) {
    m_edges[edge.id] = edge;
    m_version++;
}

std::optional<contracts::GraphNode> ModelGraphStore::get_node(const std::string& id) const {
    auto it = m_nodes.find(id);
    if (it != m_nodes.end()) return it->second;
    return std::nullopt;
}

std::vector<contracts::GraphNode> ModelGraphStore::find_nodes_by_domain(const std::string& domain) const {
    std::vector<contracts::GraphNode> results;
    for (auto const& [id, node] : m_nodes) {
        if (node.domain == domain) {
            results.push_back(node);
        }
    }
    return results;
}

uint32_t ModelGraphStore::current_version() const { return m_version; }

void ModelGraphStore::checkout_version(uint32_t version) {
    m_version = version;
}

} // namespace brain_model::core
