#ifndef OVERLAY_MANAGER_H
#define OVERLAY_MANAGER_H

#include <vector>
#include <unordered_map>
#include <string>
#include "overlay_types.h"

// Forward declarations
class Graph;

namespace model {

// Manages multiple overlay graphs and their matches to network graph
class OverlayManager {
public:
    OverlayManager(const Graph* networkGraph);
    ~OverlayManager();

    // Prevent copying - holds non-owning pointers
    OverlayManager(const OverlayManager&) = delete;
    OverlayManager& operator=(const OverlayManager&) = delete;

    // Add overlay graph (does not take ownership)
    void addOverlay(int overlayId, const Graph* overlayGraph);

    // Compute matches between network and all overlays
    // Writes to cache file, returns number of matches found
    int buildMatches(const std::string& cacheFilePath);

    // Load pre-computed matches from cache
    bool loadMatchCache(const std::string& cacheFilePath);

    // Query matches for a network node
    std::vector<OverlayMatch> getMatchesForNode(int networkNodeId) const;

    // Get all overlay edges that should be rendered
    std::vector<OverlayEdge> getOverlayEdges(int width, int height,
                                              int panX, int panY,
                                              float zoom) const;

    // Check if network node is matched to any overlay
    bool isOverlayNode(int networkNodeId) const;

    static std::string getTempFilePath();

private:
    const Graph* networkGraph_;
    std::unordered_map<int, const Graph*> overlayGraphs_;

    // Multi-map: networkNodeId -> list of matches
    std::unordered_map<int, std::vector<OverlayMatch>> matchMap_;

    void matchLabels(int overlayId, const Graph* overlayGraph);
    bool isCacheValid(const std::string& cacheFilePath) const;
};

} // namespace model

#endif // OVERLAY_MANAGER_H
