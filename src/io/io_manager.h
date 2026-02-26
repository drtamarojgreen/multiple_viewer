#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include "../map_logic.h"
#include <string>

namespace io {

class IOManager {
public:
    static bool loadJSON(Graph& graph, const std::string& filepath);
    static bool saveJSON(const Graph& graph, const std::string& filepath);
    static bool exportSVG(const Graph& graph, const std::string& filepath);
};

} // namespace io

#endif // IO_MANAGER_H
