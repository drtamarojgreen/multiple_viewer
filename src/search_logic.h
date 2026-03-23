#ifndef SEARCH_LOGIC_H
#define SEARCH_LOGIC_H

#include "map_logic.h"
#include <vector>
#include <string>

struct SearchState {
    std::string keyword;
    std::vector<int> matches;
    int activeMatchIndex = -1;
    bool isActive = false;
    bool lastSearchHadNoResults = false;

    void clear() {
        keyword.clear();
        matches.clear();
        activeMatchIndex = -1;
        isActive = false;
        lastSearchHadNoResults = false;
    }

    int getActiveMatchNodeId() const {
        if (activeMatchIndex >= 0 && activeMatchIndex < (int)matches.size()) {
            return matches[activeMatchIndex];
        }
        return -1;
    }
};

// Function to search for nodes by topic label
std::vector<int> findSimilarTopics(const Graph& graph, const std::string& searchTerm);

#endif // SEARCH_LOGIC_H
