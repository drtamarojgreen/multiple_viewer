#include "map_logic.h"
#include <queue>
#include <algorithm>
#include <chrono>

void Graph::addNode(const GraphNode& node) {
    if (nodeExists(node.index)) return;
    nodes.push_back(node);
    nodeMap[node.index] = node;
}

void Graph::removeNode(int index) {
    if (!nodeExists(index)) return;

    // Remove this node from neighbors of others
    for (auto& [otherIndex, otherNode] : nodeMap) {
        auto before = otherNode.neighbors.size();
        otherNode.neighbors.erase(
            std::remove(otherNode.neighbors.begin(), otherNode.neighbors.end(), index),
            otherNode.neighbors.end()
        );
        if (otherNode.neighbors.size() != before) {
            updateNode(otherIndex, otherNode);
        }
    }

    // Remove from nodeMap and nodes vector
    nodeMap.erase(index);
    nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
                [index](const GraphNode& node) { return node.index == index; }),
                nodes.end());

    // Also remove from focused set and positions if necessary
    focusedNodeIndices.erase(index);
    nodePos.erase(index);
}

bool Graph::nodeExists(int index) const {
    return nodeMap.find(index) != nodeMap.end();
}

// Modular function to update a node in both nodes vector and nodeMap
void Graph::updateNode(int index, const GraphNode& updatedNode) {
    // Update in nodeMap
    nodeMap[index] = updatedNode;

    // Update in nodes vector
    auto it = std::find_if(nodes.begin(), nodes.end(),
        [index](const GraphNode& n) { return n.index == index; });
    if (it != nodes.end()) {
        *it = updatedNode;
    }
}

void Graph::addEdge(int from, int to) {
    if (!nodeExists(from) || !nodeExists(to)) return;
    nodeMap[from].neighbors.push_back(to);
    nodeMap[to].neighbors.push_back(from);
    updateNode(from, nodeMap[from]);
    updateNode(to, nodeMap[to]);
}

void Graph::clear() {
    nodes.clear();
    nodeMap.clear();
    focusedNodeIndices.clear();
    summary = GraphSummary{};
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
    if (nodes.empty()) return true;
    std::set<int> visited;
    std::queue<int> q;
    q.push(nodes[0].index);
    visited.insert(nodes[0].index);

    while (!q.empty()) {
        int current = q.front(); q.pop();
        for (int neighbor : nodeMap.at(current).neighbors) {
            if (!visited.count(neighbor)) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }

    return visited.size() == nodes.size();
}

// Cull off-screen blocks
bool Graph::isInViewport(int worldX, int worldY, int blockSize) const {
    const int H = 25, W = 80;
    int half = blockSize / 2;
    int r0 = worldX - half + panY, r1 = worldX + half + panY;
    int c0 = worldY - half + panX, c1 = worldY + half + panX;
    if (r1 < 0 || r0 >= H) return false;
    if (c1 < 0 || c0 >= W) return false;
    return true;
}


int Graph::edgeCount() const {
    int count = 0;
    for (const auto& node : nodes) {
        count += node.neighbors.size();
    }
    return count / 2;  // undirected
}

float Graph::computeAvgDegree() const {
    if (nodes.empty()) return 0.0f;
    return (float)(edgeCount() * 2) / nodes.size();
}

int Graph::countIsolatedNodes() const {
    int isolated = 0;
    for (const auto& node : nodes) {
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

void Graph::setMaxRenderDistance(int d) {
    maxRenderDistance = d;
}

// BFS depth limit
int Graph::getMaxDistance() const {
    static int baseMax[6] = {0, 2, 4, 8, 10, 20};
    int d = baseMax[static_cast<int>(zoomLevel)];
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
    if (nodes.empty()) return;

    int currentFocus = -1;
    if (!focusedNodeIndices.empty()) {
        currentFocus = *focusedNodeIndices.begin();
        focusedNodeIndices.clear();
    }

    auto it = std::find_if(nodes.begin(), nodes.end(),
        [&](const GraphNode& n) { return n.index > currentFocus; });

    if (it == nodes.end()) {
        focusedNodeIndices.insert(nodes.front().index);  // wrap around to first
    } else {
        focusedNodeIndices.insert(it->index);            // next node
    }
}

void Graph::zoomIn() {
    if (zoomLevel < ZoomLevel::Z5)
        zoomLevel = static_cast<ZoomLevel>(static_cast<int>(zoomLevel) + 1);
}

void Graph::zoomOut() {
    if (zoomLevel > ZoomLevel::Z1)
        zoomLevel = static_cast<ZoomLevel>(static_cast<int>(zoomLevel) - 1);
}

void Graph::pan(int dx, int dy) {
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

// no‐arg vanishing point
VanishingPoint calculateVanishingPoint() {
    return calculateVanishingPoint(CONSOLE_WIDTH, CONSOLE_HEIGHT);
}



// 3D → 2D projection
Point2D projectToVanishingPoint(const Point3D& wp, const VanishingPoint& vp) {
    float px = vp.centerX + (wp.x * vp.focalLength) / (vp.viewDistance + wp.z);
    float py = vp.centerY + (wp.y * vp.focalLength) / (vp.viewDistance + wp.z);
    return { int(px), int(py) };
}

int calculateTotalEdges(const Graph& g) {
    int sum = 0;
    for (const auto& node : g.nodes)
        sum += static_cast<int>(node.neighbors.size());
    return sum / 2;  // assuming undirected edges, each counted twice
}

int Graph::getMaxLabelLength() const {
    int maxLen = 0;
    for (const auto& node : nodes)
        maxLen = std::max(maxLen, static_cast<int>(node.label.size()));
    return maxLen;
}

// getAdaptiveLabelLength free‐function
int getAdaptiveLabelLength(int depth, Graph::ZoomLevel zoom) {
    Graph tmp;
    tmp.zoomLevel = zoom;
    return tmp.getAdaptiveLabelLength(depth);
}

// calculateNodeSize free‐function
int calculateNodeSize(int depth, Graph::ZoomLevel zoom) {
    //Graph tmp;
    //tmp.zoomLevel = zoom;
    //return tmp.calculateNodeSize(depth);
    const int sizes[] = { 3, 4, 5, 5, 5 };  // Z1 → 3, Z2 → 4, then clamp to 5
    int idx = std::min(static_cast<int>(zoom), 4);
    return sizes[idx];
}

double calculateClusteringCoefficient(const Graph& g) {
    double total = 0.0;
    for (const auto& node : g.nodes) {
        const auto& nbrs = node.neighbors;
        int k = nbrs.size();
        if (k < 2) continue;
        int links = 0;
        for (int u : nbrs) {
            for (int v : nbrs) {
                if (u != v) {
                    // Find GraphNode for u
                    auto it_u = std::find_if(g.nodes.begin(), g.nodes.end(),
                        [u](const GraphNode& n) { return n.index == u; });
                    if (it_u != g.nodes.end()) {
                        // Check if v is a neighbor of u
                        if (std::find(it_u->neighbors.begin(), it_u->neighbors.end(), v) != it_u->neighbors.end()) {
                            links++;
                        }
                    }
                }
            }
        }
        total += links / float(k * (k - 1));
    }
    return g.nodes.empty() ? 0.0f : total / g.nodes.size();
}

// Update Cached Summary
void Graph::updateSummary() {
    summary.totalNodes = nodes.size();
    summary.totalEdges = edgeCount();
    summary.averageDegree = computeAvgDegree();
    summary.isConnected = isConnected();
    summary.isolatedNodeCount = countIsolatedNodes();

    summary.focusedNodes.clear();
    for (int idx : focusedNodeIndices) {
        summary.focusedNodes.push_back(idx);
    }

    summary.topicWeights.clear();
    std::vector<std::pair<int, int>> nodeDegrees;
    std::vector<std::pair<int, int>> subjectDegrees;

    for (const auto& node : nodes) {
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
