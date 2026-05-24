#include "genome_manager.h"
#include "../io/io_manager.h"
#include <fstream>
#include <iostream>

namespace genome {

std::string GenomeManager::cachePath_;
std::map<std::string, GeneData> GenomeManager::cache_;

void GenomeManager::initialize(const std::string& cachePath) {
    cachePath_ = cachePath;
    loadCache();
}

GeneData GenomeManager::getGeneInfo(const std::string& geneId) {
    if (cache_.count(geneId)) {
        return cache_.at(geneId);
    }
    // In a real app, this would perform a network request
    return {"", "", "No data available", {}};
}

void GenomeManager::requestGenomeData(const std::string& query) {
    std::cout << "Requesting genome data for: " << query << "..." << std::endl;
    // Mocking API response
    GeneData data = {query, query + "_Symbol", "Mock gene data for " + query, {"Metabolic", "Signaling"}};
    cache_[query] = data;
    saveCache();
    std::cout << "Data cached successfully." << std::endl;
}

void GenomeManager::loadCache() {
    // Basic implementation using the existing IOManager to load if it were JSON
    // For now, it starts empty as a stub
}

void GenomeManager::saveCache() {
    // Stub for JSON persistence
}

} // namespace genome
