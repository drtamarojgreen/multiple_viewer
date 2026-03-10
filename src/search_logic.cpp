#include "search_logic.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <cctype>

// Function to search for nodes by topic label
std::vector<int> findSimilarTopics(const Graph& graph, const std::string& searchTerm) {
    if (searchTerm.empty()) return {};

    std::vector<int> results;

    // Split search term into tokens for "AND" search
    std::vector<std::string> tokens;
    std::stringstream ss(searchTerm);
    std::string token;
    while (ss >> token) {
        std::transform(token.begin(), token.end(), token.begin(), ::tolower);
        tokens.push_back(token);
    }

    if (tokens.empty()) return {};

    for (const auto& node : graph.nodes) {
        std::string lowerLabel = node.label;
        for (auto &c : lowerLabel) c = std::tolower((unsigned char)c);

        bool allTokensMatch = true;
        for (const auto& t : tokens) {
            if (lowerLabel.find(t) == std::string::npos) {
                // Also check subject index or other attributes as a fallback scope
                std::string subjectStr = std::to_string(node.subjectIndex);
                if (subjectStr.find(t) == std::string::npos) {
                    allTokensMatch = false;
                    break;
                }
            }
        }

        if (allTokensMatch) {
            results.push_back(node.index);
        }
    }
    return results;
}
