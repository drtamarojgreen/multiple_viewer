#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include "../map_logic.h"
#include <string>
#include <set>
#include <vector>

namespace io {

class IOManager {
public:
    static bool loadJSON(Graph& graph, const std::string& filepath);
    static bool saveJSON(const Graph& graph, const std::string& filepath);
    static bool exportSVG(const Graph& graph, const std::string& filepath);

    // CSV Support
    static bool loadGraphFromCSV(Graph& graph, const std::string& filename);
    static bool saveGraphToCSV(const Graph& graph, const std::string& filename);

    // Utilities
    static std::set<int> parseNeighbors(const std::string& neighborStr);
    static std::string serializeNodeToJson(const Graph& graph, int nodeId);
};

} // namespace io

#endif // IO_MANAGER_H
