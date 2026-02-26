#include "core/contracts/IModelGraphStore.h"
#include <unordered_map>
#include <optional>

namespace brain_model::core {

using namespace brain_model::core::contracts;

class ModelGraphStore : public IModelGraphStore {
public:
    void add_node(const GraphNode& node) override {
        m_nodes[node.id] = node;
        m_version++;
    }

    void add_edge(const GraphEdge& edge) override {
        m_edges[edge.id] = edge;
        m_version++;
    }

    std::optional<GraphNode> get_node(const std::string& id) const override {
        auto it = m_nodes.find(id);
        if (it != m_nodes.end()) return it->second;
        return std::nullopt;
    }

    std::vector<GraphNode> find_nodes_by_domain(const std::string& domain) const override {
        std::vector<GraphNode> results;
        for (auto const& [id, node] : m_nodes) {
            if (node.domain == domain) {
                results.push_back(node);
            }
        }
        return results;
    }

    uint32_t current_version() const override { return m_version; }
    
    void checkout_version(uint32_t version) override {
        // Versioning would require a persistent history log or COW graph
        m_version = version;
    }

private:
    std::unordered_map<std::string, GraphNode> m_nodes;
    std::unordered_map<std::string, GraphEdge> m_edges;
    uint32_t m_version = 0;
};

} // namespace brain_model::core
