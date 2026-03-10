#include "search_logic.h"
#include <algorithm>
#include <string>
#include <cctype>

// Function to search for nodes by topic label
std::vector<int> findSimilarTopics(const Graph& graph, const std::string& searchTerm) {
    std::vector<int> results;
    if (searchTerm.empty()) return results;

    std::string s = searchTerm;
    for (auto &c : s) c = std::tolower((unsigned char)c);

    for (const auto& node : graph.nodes) {
        std::string l = node.label;
        for (auto &c : l) c = std::tolower((unsigned char)c);

        if (l.find(s) != std::string::npos) {
            results.push_back(node.index);
        }
    }
    return results;
}
