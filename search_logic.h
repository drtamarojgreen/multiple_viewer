#ifndef SEARCH_LOGIC_H
#define SEARCH_LOGIC_H

#include "map_logic.h"
#include <vector>
#include <string>

// Function to search for nodes by topic label
std::vector<int> findSimilarTopics(const Graph& graph, const std::string& searchTerm);

#endif // SEARCH_LOGIC_H
