#pragma once
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <filesystem>

namespace tp {
    struct Node {
        std::string name;
        std::set<int> neigh;
        int weight;
        int subj;
    };

    std::vector<std::string> splitBlocks(const std::string& text);
    std::vector<std::string> splitLines(const std::string& block);
    
    int processTopics(const std::filesystem::path& inDir, const std::filesystem::path& outDir);
}
