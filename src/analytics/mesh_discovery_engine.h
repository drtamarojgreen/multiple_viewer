#ifndef MESH_DISCOVERY_ENGINE_H
#define MESH_DISCOVERY_ENGINE_H

#include "../map_logic.h"
#include "worker_pool.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include <mutex>
#include <future>

namespace analytics {

struct DiscoveryConfig {
    int maxLevels = 3;
    std::map<int, int> levelThresholds = {{1, 1000}, {2, 500}, {3, 100}};
    int maxChildren = 5;
    int maxTotalTerms = 50;
};

class MeshDiscoveryEngine {
public:
    MeshDiscoveryEngine(WorkerPool& pool, DiscoveryConfig config = DiscoveryConfig());

    std::future<void> runDiscovery(Graph& graph, const std::string& seedTerm);

private:
    void discoverRecursive(Graph& graph, std::string term, int level, int parentCount);
    int fetchPublicationCount(const std::string& term);
    std::set<std::string> fetchRelatedTerms(const std::string& term);

    WorkerPool& workerPool;
    DiscoveryConfig config;
    std::set<std::string> visited;
    std::mutex visitedMutex;
    int totalTerms = 0;
    std::mutex totalTermsMutex;
};

} // namespace analytics

#endif // MESH_DISCOVERY_ENGINE_H
