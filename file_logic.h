#ifndef FILE_LOGIC_H
#define FILE_LOGIC_H

#include "map_logic.h"
#include <string>

// Load Graph from CSV
// Parses node data, subject flags, weights, and neighbor links
bool loadGraphFromCSV(Graph& graph, const std::string& filename);

// Save Graph to CSV
// Serializes all node metadata and graph structure to disk
bool saveGraphToCSV(const Graph& graph, const std::string& filename);

std::set<int> parseNeighbors(const std::string& neighborStr);
#endif

