#include "overlay_manager.h"
#include "map_logic.h"
#include <fstream>
#include <cstring>
#include <sys/stat.h>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#define PATH_SEP "\\"
#else
#include <unistd.h>
#define PATH_SEP "/"
#endif

namespace model {

OverlayManager::OverlayManager(const Graph* networkGraph)
    : networkGraph_(networkGraph) {
}

OverlayManager::~OverlayManager() {
}

void OverlayManager::addOverlay(int overlayId, const Graph* overlayGraph) {
    overlayGraphs_[overlayId] = overlayGraph;
}

int OverlayManager::buildMatches(const std::string& cacheFilePath) {
    matchMap_.clear();

    // Match each overlay against network graph
    for (const auto& pair : overlayGraphs_) {
        matchLabels(pair.first, pair.second);
    }

    // Stream write to file to avoid memory spike
    std::ofstream ofs(cacheFilePath, std::ios::binary | std::ios::trunc);
    if (!ofs.is_open()) {
        return 0;
    }

    int32_t totalMatches = 0;
    for (const auto& pair : matchMap_) {
        totalMatches += static_cast<int32_t>(pair.second.size());
    }

    ofs.write(reinterpret_cast<const char*>(&totalMatches), sizeof(totalMatches));

    for (const auto& pair : matchMap_) {
        for (const auto& match : pair.second) {
            ofs.write(reinterpret_cast<const char*>(&match), sizeof(match));
        }
    }

    ofs.close();
    return totalMatches;
}

bool OverlayManager::loadMatchCache(const std::string& cacheFilePath) {
    std::ifstream ifs(cacheFilePath, std::ios::binary);
    if (!ifs.is_open()) {
        return false;
    }

    matchMap_.clear();

    int32_t totalMatches = 0;
    ifs.read(reinterpret_cast<char*>(&totalMatches), sizeof(totalMatches));

    if (ifs.fail() || totalMatches < 0) {
        return false;
    }

    for (int32_t i = 0; i < totalMatches; ++i) {
        OverlayMatch match;
        ifs.read(reinterpret_cast<char*>(&match), sizeof(match));

        if (ifs.fail()) {
            matchMap_.clear();
            return false;
        }

        matchMap_[match.networkNodeId].push_back(match);
    }

    ifs.close();
    return true;
}

std::vector<OverlayMatch> OverlayManager::getMatchesForNode(int networkNodeId) const {
    auto it = matchMap_.find(networkNodeId);
    if (it == matchMap_.end()) {
        return std::vector<OverlayMatch>();
    }
    return it->second;
}

std::vector<OverlayEdge> OverlayManager::getOverlayEdges(int width, int height,
                                                           int panX, int panY,
                                                           float zoom) const {
    std::vector<OverlayEdge> edges;

    // For each matched network node, find overlay neighbors
    for (const auto& pair : matchMap_) {
        int networkNodeId = pair.first;

        for (const auto& match : pair.second) {
            auto it = overlayGraphs_.find(match.overlayGraphId);
            if (it == overlayGraphs_.end()) continue;

            const Graph* overlay = it->second;
            std::vector<int> neighbors = overlay->getNeighbors(match.overlayNodeId);

            for (int neighborId : neighbors) {
                OverlayEdge edge;
                edge.fromNodeId = networkNodeId;
                edge.toNodeId = neighborId;

                // Calculate screen position of neighbor
                auto pos = overlay->getNodePosition(neighborId);
                // Note: using simple projection similar to what we'll use in renderer
                edge.screenX = (pos.x + panX) * zoom;
                edge.screenY = (pos.y + panY) * zoom;

                // Check if offscreen
                edge.isOffscreen = (edge.screenX < 0 || edge.screenX >= width ||
                                    edge.screenY < 0 || edge.screenY >= height);

                edges.push_back(edge);
            }
        }
    }

    return edges;
}

bool OverlayManager::isOverlayNode(int networkNodeId) const {
    return matchMap_.find(networkNodeId) != matchMap_.end();
}

void OverlayManager::matchLabels(int overlayId, const Graph* overlayGraph) {
    if (!networkGraph_ || !overlayGraph) return;

    std::vector<int> networkNodes = networkGraph_->getAllNodeIds();
    std::vector<int> overlayNodes = overlayGraph->getAllNodeIds();

    // Build a map of labels to network node IDs for O(N) lookup
    std::unordered_map<std::string, std::vector<int>> networkLabelMap;
    for (int netId : networkNodes) {
        std::string netLabel = networkGraph_->getNodeLabel(netId);
        if (!netLabel.empty()) {
            networkLabelMap[netLabel].push_back(netId);
        }
    }

    // Match overlay nodes against the map for O(M) lookup
    for (int ovId : overlayNodes) {
        std::string ovLabel = overlayGraph->getNodeLabel(ovId);
        if (ovLabel.empty()) continue;

        auto it = networkLabelMap.find(ovLabel);
        if (it != networkLabelMap.end()) {
            for (int netId : it->second) {
                OverlayMatch match;
                match.networkNodeId = netId;
                match.overlayGraphId = overlayId;
                match.overlayNodeId = ovId;

                matchMap_[netId].push_back(match);
            }
        }
    }
}

bool OverlayManager::isCacheValid(const std::string& cacheFilePath) const {
    struct stat buffer;
    return (stat(cacheFilePath.c_str(), &buffer) == 0);
}

std::string OverlayManager::getTempFilePath() {
#ifdef _WIN32
    char tempPath[MAX_PATH];
    GetTempPathA(MAX_PATH, tempPath);
    return std::string(tempPath) + "graph_overlay_cache.bin";
#else
    const char* tmpDir = getenv("TMPDIR");
    if (!tmpDir) tmpDir = "/tmp";
    return std::string(tmpDir) + "/graph_overlay_cache.bin";
#endif
}

} // namespace model
