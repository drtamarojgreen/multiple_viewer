#include "mesh_discovery_engine.h"
#include "../io/xml_extractor.h"
#include "../io/yaml_parser.h"
#include "../logger.h"
#include <algorithm>

namespace analytics {

MeshDiscoveryEngine::MeshDiscoveryEngine(WorkerPool& pool, DiscoveryConfig cfg)
    : workerPool(pool), config(cfg) {}

void MeshDiscoveryEngine::loadConfig(const std::string& filepath) {
    auto cfg = io::YamlParser::loadSimpleYaml(filepath);
    if (cfg.empty()) {
        Logger::error("MeSH Discovery Configuration file missing or empty: " + filepath);
        return;
    }
    config.maxLevels = io::YamlParser::getInt(cfg, "max_levels", config.maxLevels);
    config.maxChildren = io::YamlParser::getInt(cfg, "max_children_per_node", config.maxChildren);
    config.maxTotalTerms = io::YamlParser::getInt(cfg, "max_total_terms", config.maxTotalTerms);
    config.levelThresholds[1] = io::YamlParser::getInt(cfg, "threshold_level_1", config.levelThresholds[1]);
    config.levelThresholds[2] = io::YamlParser::getInt(cfg, "threshold_level_2", config.levelThresholds[2]);
    config.levelThresholds[3] = io::YamlParser::getInt(cfg, "threshold_level_3", config.levelThresholds[3]);
    config.significanceThreshold = io::YamlParser::getFloat(cfg, "significance_score_threshold", config.significanceThreshold);
    config.fallbackThreshold = io::YamlParser::getInt(cfg, "fallback_threshold", config.fallbackThreshold);
}

void MeshDiscoveryEngine::loadMockData(const std::string& filepath) {
    mockData = io::YamlParser::loadSimpleYaml(filepath);
    if (mockData.empty()) {
        Logger::error("MeSH Mock Data file missing or empty: " + filepath);
    }
}

std::future<void> MeshDiscoveryEngine::runDiscovery(Graph& graph, const std::string& seedTerm) {
    visited.clear();
    totalTerms = 0;

    return std::async(std::launch::async, [this, &graph, seedTerm]() {
        discoverRecursive(graph, seedTerm, 1, 0);
        workerPool.waitAll();
    });
}

void MeshDiscoveryEngine::discoverRecursive(Graph& graph, std::string term, int level, int parentCount) {
    if (level > config.maxLevels) return;

    {
        std::lock_guard<std::mutex> lock(totalTermsMutex);
        if (totalTerms >= config.maxTotalTerms) return;
    }

    {
        std::lock_guard<std::mutex> lock(visitedMutex);
        if (visited.count(term)) return;
        visited.insert(term);
    }

    int count = fetchPublicationCount(term);

    float significance = 1.0f;
    if (parentCount > 0) significance = static_cast<float>(count) / parentCount;

    int threshold = config.levelThresholds.count(level) ? config.levelThresholds.at(level) : config.fallbackThreshold;
    bool isSignificant = (count >= threshold) || (parentCount > 0 && significance > config.significanceThreshold);

    if (!isSignificant) return;

    {
        std::lock_guard<std::mutex> lock(totalTermsMutex);
        totalTerms++;
    }

    // Add to graph (mocking index/subject for now)
    static std::atomic<int> nextIdx(1000);
    int idx = nextIdx++;
    GraphNode node(term, idx);
    node.weight = count;
    graph.addNode(node);

    if (level < config.maxLevels) {
        std::set<std::string> related = fetchRelatedTerms(term);
        int processed = 0;
        for (const auto& rel : related) {
            if (processed >= config.maxChildren) break;
            workerPool.enqueue([this, &graph, rel, level, count]() {
                discoverRecursive(graph, rel, level + 1, count);
            });
            processed++;
        }
    }
}

int MeshDiscoveryEngine::fetchPublicationCount(const std::string& term) {
    std::string key = "count_" + term;
    if (mockData.count(key)) {
        try {
            return std::stoi(mockData.at(key));
        } catch (...) {}
    }

    // Demonstrate usage of internal logic even if network is stubbed
    std::string mockXml = "<PubmedArticleSet><PubmedArticle><DescriptorName>" + term + "</DescriptorName></PubmedArticle></PubmedArticleSet>";
    auto extracted = io::XmlExtractor::extractMeshTerms(mockXml);
    if (!extracted.empty()) return config.fallbackThreshold;

    return config.fallbackThreshold / 2;
}

std::set<std::string> MeshDiscoveryEngine::fetchRelatedTerms(const std::string& term) {
    std::string key = "related_" + term;
    if (mockData.count(key)) {
        return io::XmlExtractor::extractMeshTerms(mockData.at(key));
    }
    return {};
}

} // namespace analytics
