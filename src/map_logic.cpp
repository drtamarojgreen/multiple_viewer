#include "map_logic.h"
#include <queue>
#include <algorithm>
#include <chrono>
#include <cmath>

void Graph::addNode(const GraphNode& node) {
    if (nodeExists(node.index)) return;
    nodeMap[node.index] = node;
}

void Graph::removeNode(int index) {
    if (!nodeExists(index)) return;

    // Remove this node from neighbors of others
    for (auto& [otherIndex, otherNode] : nodeMap) {
        auto& nbrs = otherNode.neighbors;
        auto it = std::find(nbrs.begin(), nbrs.end(), index);
        if (it != nbrs.end()) {
            nbrs.erase(it);
        }
    }

    // Remove from nodeMap
    nodeMap.erase(index);

    // Also remove from focused set and positions if necessary
    focusedNodeIndices.erase(index);
    nodePos.erase(index);
}

bool Graph::nodeExists(int index) const {
    return nodeMap.find(index) != nodeMap.end();
}

// Update a node in nodeMap
void Graph::updateNode(int index, const GraphNode& updatedNode) {
    nodeMap[index] = updatedNode;
}

void Graph::addEdge(int from, int to) {
    if (!nodeExists(from) || !nodeExists(to)) return;

    // Check if edge already exists to avoid duplicates
    auto& n1 = nodeMap[from].neighbors;
    if (std::find(n1.begin(), n1.end(), to) == n1.end()) {
        n1.push_back(to);
        updateNode(from, nodeMap[from]);
    }

    auto& n2 = nodeMap[to].neighbors;
    if (std::find(n2.begin(), n2.end(), from) == n2.end()) {
        n2.push_back(from);
        updateNode(to, nodeMap[to]);
    }
}

void Graph::clear() {
    nodeMap.clear();
    focusedNodeIndices.clear();
    summary = GraphSummary{};
    needsLayoutReset = true;
}

// BFS Shortest Path
std::unordered_map<int, int> Graph::calculateShortestPaths(int fromIndex) const {
    std::unordered_map<int, int> distance;
    std::queue<int> q;
    q.push(fromIndex);
    distance[fromIndex] = 0;

    while (!q.empty()) {
        int current = q.front(); q.pop();
        const GraphNode& node = nodeMap.at(current);
        for (int neighbor : node.neighbors) {
            if (distance.find(neighbor) == distance.end()) {
                distance[neighbor] = distance[current] + 1;
                q.push(neighbor);
            }
        }
    }

    return distance;
}

// Connectivity Checks
bool Graph::isConnected() const {
    if (nodeMap.empty()) return true;
    std::set<int> visited;
    std::queue<int> q;
    int firstNodeIndex = nodeMap.begin()->first;
    q.push(firstNodeIndex);
    visited.insert(firstNodeIndex);

    while (!q.empty()) {
        int current = q.front(); q.pop();
        for (int neighbor : nodeMap.at(current).neighbors) {
            if (!visited.count(neighbor)) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }

    return visited.size() == nodeMap.size();
}

// Cull off-screen blocks
bool Graph::isInViewport(int worldX, int worldY, int blockSize, const ViewContext& view) const {
    int half = blockSize / 2;
    int r0 = worldX - half + view.panY, r1 = worldX + half + view.panY;
    int c0 = worldY - half + view.panX, c1 = worldY + half + view.panX;
    if (r1 < 0 || r0 >= view.height) return false;
    if (c1 < 0 || c0 >= view.width) return false;
    return true;
}


int Graph::edgeCount() const {
    int count = 0;
    for (const auto& [id, node] : nodeMap) {
        count += node.neighbors.size();
    }
    return count / 2;  // undirected
}

float Graph::computeAvgDegree() const {
    if (nodeMap.empty()) return 0.0f;
    return (float)(edgeCount() * 2) / nodeMap.size();
}

int Graph::countIsolatedNodes() const {
    int isolated = 0;
    for (const auto& [id, node] : nodeMap) {
        if (node.neighbors.empty()) ++isolated;
    }
    return isolated;
}

std::vector<int> extractTopIndices(const std::vector<std::pair<int, int>>& list, int count) {
    std::vector<int> result;
    for (int i = 0; i < std::min(count, (int)list.size()); ++i) {
        result.push_back(list[i].first);
    }
    return result;
}

void Graph::addFocus(int idx) {
    if (!nodeExists(idx)) return;
    if (Config::allowMultiFocus) {
        focusedNodeIndices.insert(idx);
        focusedNodeIndex = idx; 
    } else {
        focusedNodeIndices.clear();
        focusedNodeIndices.insert(idx);
        focusedNodeIndex = idx;
    }
}

void Graph::removeFocus(int idx) {
    focusedNodeIndices.erase(idx);
}

void Graph::clearFocuses() {
    focusedNodeIndices.clear();
}

// BFS depth limit
int Graph::getMaxDistance(ZoomLevel zoom) const {
    static int baseMax[6] = {0, 2, 4, 8, 10, 20};
    int d = baseMax[static_cast<int>(zoom)];
    if (summary.totalNodes > 500) {
        int adjust = static_cast<int>(summary.averageDegree / 2.0);
        d = std::max(1, d - adjust);
    }
    return d;
}

// Wait for key
void Graph::pause() const {
    if (Config::quietMode) return;
    std::cout<<"Press any key to continue...";
    std::cin.get();
    std::cout<<"\n";
}


std::unordered_map<int,int> Graph::computeMultiFocusDistances() const {
    // multi-source BFS
    std::unordered_map<int,int> dist;
    std::queue<int> q;
    // initialize all focuses at distance 0
    for (int f : focusedNodeIndices) {
        dist[f] = 0;
        q.push(f);
    }
    while (!q.empty()) {
        int u = q.front(); q.pop();
        int d = dist[u];
        for (int nbr : nodeMap.at(u).neighbors) {
            // if not seen OR we found a shorter path
            if (!dist.count(nbr) || d+1 < dist[nbr]) {
                dist[nbr] = d+1;
                q.push(nbr);
            }
        }
    }
    return dist;
}

// Navigation
void Graph::cycleFocus() {
    if (nodeMap.empty()) return;

    int currentFocus = -1;
    if (!focusedNodeIndices.empty()) {
        currentFocus = *focusedNodeIndices.begin();
        focusedNodeIndices.clear();
    }

    int nextFocus = -1;
    int minIndex = -1;

    for (const auto& [id, node] : nodeMap) {
        if (minIndex == -1 || id < minIndex) minIndex = id;
        if (id > currentFocus) {
            if (nextFocus == -1 || id < nextFocus) nextFocus = id;
        }
    }

    if (nextFocus == -1) {
        focusedNodeIndices.insert(minIndex);  // wrap around
    } else {
        focusedNodeIndices.insert(nextFocus);
    }
}

void ViewContext::zoomIn() {
    if (zoomLevel < ZoomLevel::Z5)
        zoomLevel = static_cast<ZoomLevel>(static_cast<int>(zoomLevel) + 1);
}

void ViewContext::zoomOut() {
    if (zoomLevel > ZoomLevel::Z1)
        zoomLevel = static_cast<ZoomLevel>(static_cast<int>(zoomLevel) - 1);
}

void ViewContext::pan(int dx, int dy) {
    panX += dx;
    panY += dy;
}

// Adaptive node spacing
float calculateAdaptiveNodeSpacing(const Graph& g) {
    float base = 5.0f;
    float df = 1.0f + g.summary.density * 2.0f;
    float degF= (g.summary.maximumDegree>0
               ? (g.summary.averageDegree / g.summary.maximumDegree)
               : 1.0f);
    float compF= 1.0f + g.summary.components.size()*0.1f;
    return base * df * degF * compF;
}

// Density-based toggles (stub)
void applyDensityStrategy(Graph& g) {
    // toggle showLines based on density threshold
    g.showLines = (g.summary.density <= 0.7);
}


// Vanishing point setup
VanishingPoint calculateVanishingPoint(int screenW, int screenH) {
    VanishingPoint vp;
    vp.centerX     = screenW / 2.0f;
    vp.centerY     = screenH / 2.0f;
    vp.focalLength = std::min(screenW, screenH) * 0.5f;
    vp.viewDistance= vp.focalLength * 2.0f;
    return vp;
}



// 3D → 2D projection
Point2D projectToVanishingPoint(const Point3D& wp, const VanishingPoint& vp) {
    float px = vp.centerX + (wp.x * vp.focalLength) / (vp.viewDistance + wp.z);
    float py = vp.centerY + (wp.y * vp.focalLength) / (vp.viewDistance + wp.z);
    return { px, py };
}

int calculateTotalEdges(const Graph& g) {
    int sum = 0;
    for (const auto& [id, node] : g.nodeMap)
        sum += static_cast<int>(node.neighbors.size());
    return sum / 2;  // assuming undirected edges, each counted twice
}

int calculateGraphDiameter(const Graph& g) {
    if (g.nodeMap.empty()) return 0;
    int maxDist = 0;
    for (const auto& [id, startNode] : g.nodeMap) {
        auto dists = g.calculateShortestPaths(id);
        for (const auto& [nid, d] : dists) {
            if (d > maxDist) maxDist = d;
        }
    }
    return maxDist;
}

int Graph::getMaxLabelLength() const {
    int maxLen = 0;
    for (const auto& [id, node] : nodeMap)
        maxLen = std::max(maxLen, static_cast<int>(node.label.size()));
    return maxLen;
}

// getAdaptiveLabelLength member implementation
int Graph::getAdaptiveLabelLength(int depth, ZoomLevel zoom) const {
    int base = getMaxLabelLength();
    if (base == 0) base = 10; // Default if empty
    float factor = (depth == 0 ? 1.5f : (depth == 1 ? 1.0f : 0.5f));
    return std::max(3, int(base * factor));
}

// getAdaptiveLabelLength free‐function wrapper
int getAdaptiveLabelLength(int depth, ZoomLevel zoom, int baseLen) {
    float factor = (depth == 0 ? 1.5f : (depth == 1 ? 1.0f : 0.5f));
    return std::max(3, int(baseLen * factor));
}

// calculateNodeSize member implementation
int Graph::calculateNodeSize(int depth, ZoomLevel zoom) const {
    int base_size = static_cast<int>(zoom) + 1;
    return std::max(1, base_size - depth);
}

// calculateNodeSize free‐function wrapper
int calculateNodeSize(int depth, ZoomLevel zoom) {
    int base_size = static_cast<int>(zoom) + 1;
    return std::max(1, base_size - depth);
}

double calculateClusteringCoefficient(const Graph& g) {
    double total = 0.0;
    for (const auto& [id, node] : g.nodeMap) {
        const auto& nbrs = node.neighbors;
        int k = nbrs.size();
        if (k < 2) continue;
        int links = 0;
        for (int u : nbrs) {
            for (int v : nbrs) {
                if (u != v) {
                    // Find GraphNode for u
                    auto it_u = g.nodeMap.find(u);
                    if (it_u != g.nodeMap.end()) {
                        // Check if v is a neighbor of u
                        const auto& u_nbrs = it_u->second.neighbors;
                        if (std::find(u_nbrs.begin(), u_nbrs.end(), v) != u_nbrs.end()) {
                            links++;
                        }
                    }
                }
            }
        }
        total += links / float(k * (k - 1));
    }
    return g.nodeMap.empty() ? 0.0f : total / g.nodeMap.size();
}

// Update Cached Summary
void Graph::updateSummary() {
    summary.totalNodes = nodeMap.size();
    summary.totalEdges = edgeCount();
    summary.averageDegree = computeAvgDegree();
    summary.isConnected = isConnected();
    summary.isolatedNodeCount = countIsolatedNodes();
    summary.avgClusteringCoeff = calculateClusteringCoefficient(*this);
    summary.diameter = calculateGraphDiameter(*this);
    if (summary.totalNodes > 1) {
        summary.density = (2.0f * summary.totalEdges) / (summary.totalNodes * (summary.totalNodes - 1));
    } else {
        summary.density = 0.0f;
    }

    summary.focusedNodes.clear();
    for (int idx : focusedNodeIndices) {
        summary.focusedNodes.push_back(idx);
    }

    summary.topicWeights.clear();
    std::vector<std::pair<int, int>> nodeDegrees;
    std::vector<std::pair<int, int>> subjectDegrees;

    for (const auto& [id, node] : nodeMap) {
        int deg = node.neighbors.size();
        nodeDegrees.push_back({node.index, deg});
        if (node.subjectIndex >= 0) {
            subjectDegrees.push_back({node.index, deg});
        }
        summary.topicWeights[node.index] = node.weight;
    }

    auto byHigh = [](auto a, auto b) { return a.second > b.second; };
    auto byLow  = [](auto a, auto b) { return a.second < b.second; };

    std::sort(nodeDegrees.begin(), nodeDegrees.end(), byHigh);
    std::sort(subjectDegrees.begin(), subjectDegrees.end(), byHigh);
    summary.topConnectedNodes = extractTopIndices(nodeDegrees, 3);
    summary.topConnectedSubjects = extractTopIndices(subjectDegrees, 3);

    std::sort(nodeDegrees.begin(), nodeDegrees.end(), byLow);
    std::sort(subjectDegrees.begin(), subjectDegrees.end(), byLow);
    summary.leastConnectedNodes = extractTopIndices(nodeDegrees, 3);
    summary.leastConnectedSubjects = extractTopIndices(subjectDegrees, 3);
}

bool Graph::isNodeFocused(int index) const {
    return focusedNodeIndices.count(index) > 0;
}

// subject filter
bool Graph::passesSubjectFilter(int nodeId) const {
    if (!subjectFilterOnly) return true;
    if (focusedNodeIndex < 0) return true;
    return nodeMap.at(nodeId).subjectIndex
         == nodeMap.at(focusedNodeIndex).subjectIndex;
}

// focus-only mode
bool Graph::isFocusOnlyView(ZoomLevel zoom) const {
    return focusOnlyAtMaxZoom && zoom == ZoomLevel::Z5;
}

// proximity depth
float Graph::getProximityDepth(int nodeId, int width, int height) const {
    auto it = nodePos.find(nodeId);
    if (it == nodePos.end()) return 1.0f;
    float cx = width / 2.0f, cy = height / 2.0f;
    float dx = it->second.y - cx;
    float dy = it->second.x - cy;
    float dist = std::sqrt(dx*dx + dy*dy);
    return std::min(1.0f, dist / std::max(cx, cy));
}
