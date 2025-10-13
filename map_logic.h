#ifndef MAP_LOGIC_H
#define MAP_LOGIC_H

#include <vector>
#include <set>
#include <unordered_map>
#include <string>
#include <iostream>
#include <map>

// Console dimensions (shared globally)
static constexpr int CONSOLE_WIDTH  = 80;
static constexpr int CONSOLE_HEIGHT = 25;

// === Configuration Flags (formerly config.h) ===
namespace Config {
    inline bool showAnalyticsPanel = false;
    inline bool viewerOverlayMode = false;
    inline bool autoScaleDepth = true;
    inline bool showTopicWeights = true;
    inline bool allowMultiFocus = false;
    inline int panOffsetX = 0;
    inline int panOffsetY = 0;
    inline float viewerZoom = 1.0f;
    inline int nodePadding = 1;
    inline bool quietMode = false;
    inline bool allowMultiFocus = true; 
}

// === RenderBuffer (formerly render_buffer.h) ===
class RenderBuffer {
public:
    std::vector<std::string> lines;
    void clear() { lines.clear(); }
    void pushLine(const std::string& line) { lines.push_back(line); }
    void flushToTerminal() const {
        for (auto& l : lines) std::cout << l << "\n";
    }
};

// Different visualization layouts (from viewer_logic.h)
enum ViewMode {
    VM_PERSPECTIVE,  // full BFS‐based 3D perspective
    VM_TABBED,       // show depth tabs (with truncated names)
    VM_PAGED,        // render one depth “page” at a time
    VM_BOOK,         // render by subject‐chapter grouping
    VM_GRID,         // uniform grid layers
    VM_NEXUS_FLOW,   // force-directed animated layout
    VM_COUNT
};

// 3D coordinate for node placement and depth sorting
struct Coord3 {
    int x, y, z;
    bool operator<(const Coord3& o) const {
        if (z!=o.z) return z<o.z;
        if (y!=o.y) return y<o.y;
        return x<o.x;
    }
};

// Graph Data Structures
struct GraphNode {
    std::string label;
    int index;
    std::vector<int> neighbors;
    int weight = 1;
    int subjectIndex = -1;  
};

struct GraphSummary {
    int totalNodes = 0;
    int totalEdges = 0;
    float averageDegree = 0.0f;
    float maximumDegree = 0.0f;
    float density = 0.0f;
    std::vector<int> components;

    bool isConnected = false;
    int isolatedNodeCount = 0;
    std::vector<int> focusedNodes;
    std::unordered_map<int, int> topicWeights;
    std::vector<int> topConnectedNodes;
    std::vector<int> topConnectedSubjects;
    std::vector<int> leastConnectedNodes;
    std::vector<int> leastConnectedSubjects;
    double timeToLoadMs = 0;
    double timeToRenderMs = 0;
};

class Graph {
public:
    std::vector<GraphNode> nodes;
    std::unordered_map<int, GraphNode> nodeMap;
    std::map<int,Coord3>      nodePos;
    std::set<int> focusedNodeIndices;
    int focusedNodeIndex = 0;
    int panX=0, panY=0;
    GraphSummary summary;
    int maxRenderDistance = 3;

    enum class ZoomLevel { Z1, Z2, Z3, Z4, Z5 };
    ZoomLevel zoomLevel = ZoomLevel::Z3;
    // Phase II viewer/editor toggles
    bool subjectFilterOnly;
    bool focusOnlyAtMaxZoom;
    bool showLines;         // <-- newly added

    Graph() : subjectFilterOnly(false), focusOnlyAtMaxZoom(false), showLines(true) {}


    int  getMaxDistance()    const;
    int  getMaxLabelLength() const;
    bool isInViewport(int worldX, int worldY, int size) const;

    void cycleFocus();
    void zoomIn();
    void zoomOut();
    void pan(int dx, int dy);

    void editEdges();

    void addFocus(int index);
    void removeFocus(int index);
    void clearFocuses();
    void setMaxRenderDistance(int d);
    // 4) Continuous zoom block sizing
    int  calculateNodeSize(int depth) const;
    std::unordered_map<int,int> computeMultiFocusDistances() const;

    void addNode(const GraphNode& node);
    void removeNode(int index);
    void updateNode(int index, const GraphNode& updatedNode);
    bool nodeExists(int index) const;
    void addEdge(int from, int to);
    void clear();
    // Analysis
    std::unordered_map<int,int> calculateShortestPaths(int fromIndex) const;
    bool isConnected() const;
    int edgeCount() const;
    float computeAvgDegree() const;
    int countIsolatedNodes() const;
    int getMaxRenderDistance() const { return maxRenderDistance; }
    void pause() const;

    // Summary
    void updateSummary();
    // 1) Adaptive label length
    int  getAdaptiveLabelLength(int depth) const;
    // 2) Subject filtering
    bool passesSubjectFilter(int nodeId) const;
    // 3) Focus-only view at max zoom
    bool isFocusOnlyView() const;
    // 5) Proximity-based depth
    float getProximityDepth(int nodeId) const;

    // --- View Mode State ---
    ViewMode currentViewMode = VM_PERSPECTIVE;
    bool needsLayoutReset = true;
    bool isNodeFocused(int index) const;
};


// Perspective projection
struct VanishingPoint {
    float centerX, centerY;
    float focalLength;
    float viewDistance;
};

// Simple 3D/2D point types for perspective projection
struct Point3D { float x, y, z; };
struct Point2D { int x, y; };

// Grouping for "Book­-Based" view
struct BookChapter {
    std::vector<int> nodeIds;
    int              chapterDepth;
    std::string      chapterTitle;
};

// Free helper functions
// Book-based structure
std::vector<BookChapter> createBookStructure(const Graph& graph);

// Grid-based layering
int getGridLayer(int nodeIndex, int layerCount);


VanishingPoint calculateVanishingPoint(int screenW, int screenH);
VanishingPoint calculateVanishingPoint(); // no-arg version
Point2D      projectToVanishingPoint(const Point3D& wp, const VanishingPoint& vp);

// Z-buffer occlusion
void updateZBuffer(std::vector<std::vector<float>>& zbuf,
                   Point2D center, int size, float depth);

// Adaptive spacing & density strategy
float calculateAdaptiveNodeSpacing(const Graph& graph);
void  applyDensityStrategy(Graph& graph);

// Advanced summary metrics
double calculateClusteringCoefficient(const Graph& graph);
int    calculateGraphDiameter(const Graph& graph);
int    calculateTotalEdges(const Graph& graph);

#endif // MAP_LOGIC_H


