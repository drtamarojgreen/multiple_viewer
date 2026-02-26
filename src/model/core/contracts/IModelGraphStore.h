#pragma once

#include <string>
#include <vector>
#include <map>
#include <variant>
#include <optional>
#include <cstdint>
<<<<<<< HEAD

=======
>>>>>>> main

namespace brain_model::core::contracts {

using AttributeValue = std::variant<int, float, std::string, bool>;

struct GraphNode {
    std::string id;
    std::string domain;
    std::string type;
    std::map<std::string, AttributeValue> attributes;
};

struct GraphEdge {
    std::string id;
    std::string source_id;
    std::string target_id;
    std::string domain;
    std::map<std::string, AttributeValue> attributes;
};

class IModelGraphStore {
public:
    virtual ~IModelGraphStore() = default;

    // Entity Management
    virtual void add_node(const GraphNode& node) = 0;
    virtual void add_edge(const GraphEdge& edge) = 0;
    
    // Query API
    virtual std::optional<GraphNode> get_node(const std::string& id) const = 0;
    virtual std::vector<GraphNode> find_nodes_by_domain(const std::string& domain) const = 0;
    
    // Versioning
    virtual uint32_t current_version() const = 0;
    virtual void checkout_version(uint32_t version) = 0;
};

} // namespace brain_model::core::contracts
