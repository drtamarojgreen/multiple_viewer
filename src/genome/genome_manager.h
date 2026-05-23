#ifndef GENOME_MANAGER_H
#define GENOME_MANAGER_H

#include <string>
#include <map>
#include <vector>

namespace genome {

struct GeneData {
    std::string id;
    std::string symbol;
    std::string description;
    std::vector<std::string> pathways;
};

class GenomeManager {
public:
    static void initialize(const std::string& cachePath);
    static GeneData getGeneInfo(const std::string& geneId);
    static void requestGenomeData(const std::string& query);

private:
    static std::string cachePath_;
    static std::map<std::string, GeneData> cache_;
    static void loadCache();
    static void saveCache();
};

} // namespace genome

#endif // GENOME_MANAGER_H
