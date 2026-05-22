#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include "../map_logic.h"
#include <string>
#include <set>
#include <vector>

namespace io {

class StorageBackend {
public:
    virtual ~StorageBackend() = default;
    virtual bool read(const std::string& path, std::string& outData) = 0;
    virtual bool write(const std::string& path, const std::string& inData) = 0;
};

class LocalFS : public StorageBackend {
public:
    bool read(const std::string& path, std::string& outData) override;
    bool write(const std::string& path, const std::string& inData) override;
};

class IOManager {
public:
    static void setBackend(StorageBackend* backend);
    static bool loadJSON(Graph& graph, const std::string& filepath);
    static bool loadMeshJSON(Graph& graph, const std::string& filepath);
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
