#ifndef ANALYTICS_ENGINE_EXT_H
#define ANALYTICS_ENGINE_EXT_H

#include "../map_logic.h"
#include <vector>
#include <map>

namespace analytics {

struct CentralityMetrics {
    std::map<int, float> degreeCentrality;
    std::map<int, float> betweennessCentrality;
    std::map<int, float> closenessCentrality;
};

class AnalyticsEngine {
public:
    static CentralityMetrics computeCentrality(const Graph& graph);
    static std::vector<std::vector<int>> detectCommunities(const Graph& graph);
};

} // namespace analytics

#endif // ANALYTICS_ENGINE_EXT_H
