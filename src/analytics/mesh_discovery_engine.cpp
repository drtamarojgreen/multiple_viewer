#include "mesh_discovery_engine.h"
#include "../io/xml_extractor.h"
#include "../logger.h"
#include <algorithm>

namespace analytics {

MeshDiscoveryEngine::MeshDiscoveryEngine(WorkerPool& pool, DiscoveryConfig cfg)
    : workerPool(pool), config(cfg) {}

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

    int threshold = config.levelThresholds.count(level) ? config.levelThresholds.at(level) : 100;
    bool isSignificant = (count >= threshold) || (parentCount > 0 && significance > 0.1f);

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
    // Stub for actual API call
    if (term == "Alzheimer") return 50000;
    if (term == "Amyloid") return 20000;
    if (term == "Tau") return 15000;

    // Demonstrate usage of internal logic even if network is stubbed
    std::string mockXml = "<PubmedArticleSet><PubmedArticle><DescriptorName>" + term + "</DescriptorName></PubmedArticle></PubmedArticleSet>";
    auto extracted = io::XmlExtractor::extractMeshTerms(mockXml);
    if (!extracted.empty()) return 100;

    return 50;
}

std::set<std::string> MeshDiscoveryEngine::fetchRelatedTerms(const std::string& term) {
    // In a real implementation, this would fetch from NCBI and use XmlExtractor.
    // For this integration, we demonstrate the architectural link.
    std::string mockXml;
    if (term == "Alzheimer") {
        mockXml = "<PubmedArticleSet><PubmedArticle><DescriptorName>Amyloid</DescriptorName><DescriptorName>Tau</DescriptorName><DescriptorName>Dementia</DescriptorName></PubmedArticle></PubmedArticleSet>";
    } else if (term == "Amyloid") {
        mockXml = "<PubmedArticleSet><PubmedArticle><DescriptorName>Plaques</DescriptorName><DescriptorName>Protein Aggregation</DescriptorName></PubmedArticle></PubmedArticleSet>";
    } else {
        return {};
    }

    return io::XmlExtractor::extractMeshTerms(mockXml);
}

} // namespace analytics
