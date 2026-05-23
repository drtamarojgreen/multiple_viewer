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
    int maxLevels = 0;
    std::map<int, int> levelThresholds;
    int maxChildren = 0;
    int maxTotalTerms = 0;
    float significanceThreshold = 0.0f;
    int fallbackThreshold = 0;
};

class MeshDiscoveryEngine {
public:
    MeshDiscoveryEngine(WorkerPool& pool, DiscoveryConfig config = DiscoveryConfig());

    void loadConfig(const std::string& filepath);
    void loadMockData(const std::string& filepath);
    std::future<void> runDiscovery(Graph& graph, const std::string& seedTerm);

private:
    void discoverRecursive(Graph& graph, std::string term, int level, int parentCount);
    int fetchPublicationCount(const std::string& term);
    std::set<std::string> fetchRelatedTerms(const std::string& term);

    WorkerPool& workerPool;
    DiscoveryConfig config;
    std::map<std::string, std::string> mockData;
    std::set<std::string> visited;
    std::mutex visitedMutex;
    int totalTerms = 0;
    std::mutex totalTermsMutex;
};

} // namespace analytics

#endif // MESH_DISCOVERY_ENGINE_H
