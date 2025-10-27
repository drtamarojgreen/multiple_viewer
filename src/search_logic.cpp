#include "search_logic.h"
#include "acronyms.h"
#include <algorithm>
#include <string>

// Function to search for nodes by topic label
std::vector<int> findSimilarTopics(const Graph& graph, const std::string& searchTerm) {
    std::vector<int> results;
    std::string lowerSearchTerm = searchTerm;
    std::transform(lowerSearchTerm.begin(), lowerSearchTerm.end(), lowerSearchTerm.begin(), ::tolower);

    for (const auto& node : graph.nodes) {
        std::string lowerLabel = node.label;
        std::transform(lowerLabel.begin(), lowerLabel.end(), lowerLabel.begin(), ::tolower);

        if (lowerLabel.find(lowerSearchTerm) != std::string::npos) {
            results.push_back(node.index);
        }
    }
    return results;
}
