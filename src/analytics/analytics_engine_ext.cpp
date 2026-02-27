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
    std::set<int> visited;

    for (const auto& [id, node] : graph.nodeMap) {
        if (visited.count(id)) continue;

        std::vector<int> currentComp;
        std::vector<int> q = {id};
        visited.insert(id);

        while(!q.empty()) {
            int u = q.back(); q.pop_back();
            currentComp.push_back(u);
            const auto& nNode = graph.nodeMap.at(u);
            for(int v : nNode.neighbors) {
                if(!visited.count(v)) {
                    visited.insert(v);
                    q.push_back(v);
                }
            }
        }
        communities.push_back(currentComp);
    }
    return communities;
}

} // namespace analytics
