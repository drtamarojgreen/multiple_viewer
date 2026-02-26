#include "analytics_engine_ext.h"
#include <cmath>

namespace analytics {

CentralityMetrics AnalyticsEngine::computeCentrality(const Graph& graph) {
    CentralityMetrics metrics;
    int n = graph.nodes.size();
    if (n == 0) return metrics;

    for (const auto& [id, node] : graph.nodeMap) {
        metrics.degreeCentrality[id] = static_cast<float>(node.neighbors.size()) / (n - 1);
        metrics.betweennessCentrality[id] = 0.0f; // Placeholder for full Brandes algorithm
        metrics.closenessCentrality[id] = 0.0f;
    }
    return metrics;
}

std::vector<std::vector<int>> AnalyticsEngine::detectCommunities(const Graph& graph) {
    std::vector<std::vector<int>> communities;
    // Simple mock community detection: group by subject index
    std::map<int, std::vector<int>> groups;
    for (const auto& [id, node] : graph.nodeMap) {
        groups[node.subjectIndex].push_back(id);
    }
    for (auto& pair : groups) {
        communities.push_back(pair.second);
    }
    return communities;
}

} // namespace analytics
