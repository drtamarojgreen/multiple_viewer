// analysis_logic.cpp
#include "analysis_logic.h"
#include <iostream>
#include <algorithm>
#include <queue>
#include <cmath>

void AnalyticsEngine::runFullAnalysis(Graph& g) {
    GraphSummary& s = g.summary;
    s.totalNodes = g.nodes.size();
    s.totalEdges = g.edgeCount();
    s.averageDegree = g.computeAvgDegree();
    s.isConnected = g.isConnected();
    s.isolatedNodeCount = g.countIsolatedNodes();
    s.avgClusteringCoeff = calculateClusteringCoefficient(g);
    s.diameter = calculateGraphDiameter(g);
    s.density = calculateDensity(g);
    s.components = findComponents(g);

    s.focusedNodes.clear();
    for (int idx : g.focusedNodeIndices) {
        s.focusedNodes.push_back(idx);
    }

    s.topicWeights.clear();
    std::vector<std::pair<int, int>> nodeDegrees;
    std::vector<std::pair<int, int>> subjectDegrees;

    for (const auto& node : g.nodes) {
        int deg = node.neighbors.size();
        nodeDegrees.push_back({node.index, deg});
        if (node.subjectIndex >= 0) {
            subjectDegrees.push_back({node.index, deg});
        }
        s.topicWeights[node.index] = node.weight;
    }

    auto byHigh = [](auto a, auto b) { return a.second > b.second; };
    auto byLow  = [](auto a, auto b) { return a.second < b.second; };

    std::sort(nodeDegrees.begin(), nodeDegrees.end(), byHigh);
    std::sort(subjectDegrees.begin(), subjectDegrees.end(), byHigh);
    s.topConnectedNodes = extractTopIndices(nodeDegrees, 3);
    s.topConnectedSubjects = extractTopIndices(subjectDegrees, 3);

    std::sort(nodeDegrees.begin(), nodeDegrees.end(), byLow);
    std::sort(subjectDegrees.begin(), subjectDegrees.end(), byLow);
    s.leastConnectedNodes = extractTopIndices(nodeDegrees, 3);
    s.leastConnectedSubjects = extractTopIndices(subjectDegrees, 3);
}

double AnalyticsEngine::calculateClusteringCoefficient(const Graph& g) {
    double total = 0.0;
    if (g.nodes.empty()) return 0.0;

    for (const auto& node : g.nodes) {
        const auto& nbrs = node.neighbors;
        int k = nbrs.size();
        if (k < 2) continue;
        int links = 0;
        for (int u : nbrs) {
            for (int v : nbrs) {
                if (u != v) {
                    auto it_u = g.nodeMap.find(u);
                    if (it_u != g.nodeMap.end()) {
                        const auto& u_nbrs = it_u->second.neighbors;
                        if (std::find(u_nbrs.begin(), u_nbrs.end(), v) != u_nbrs.end()) {
                            links++;
                        }
                    }
                }
            }
        }
        total += (double)links / (k * (k - 1));
    }
    return total / g.nodes.size();
}

int AnalyticsEngine::calculateGraphDiameter(const Graph& g) {
    if (g.nodes.empty()) return 0;
    int maxDist = 0;
    for (const auto& startNode : g.nodes) {
        auto dists = g.calculateShortestPaths(startNode.index);
        for (const auto& [nid, d] : dists) {
            if (d > maxDist) maxDist = d;
        }
    }
    return maxDist;
}

float AnalyticsEngine::calculateDensity(const Graph& g) {
    if (g.nodes.size() <= 1) return 0.0f;
    return (2.0f * g.edgeCount()) / (g.nodes.size() * (g.nodes.size() - 1));
}

std::vector<int> AnalyticsEngine::findComponents(const Graph& g) {
    std::vector<int> componentCounts;
    std::set<int> visited;
    for (const auto& node : g.nodes) {
        if (visited.count(node.index)) continue;

        int count = 0;
        std::queue<int> q;
        q.push(node.index);
        visited.insert(node.index);
        while (!q.empty()) {
            int curr = q.front(); q.pop();
            count++;
            for (int nbr : g.nodeMap.at(curr).neighbors) {
                if (!visited.count(nbr)) {
                    visited.insert(nbr);
                    q.push(nbr);
                }
            }
        }
        componentCounts.push_back(count);
    }
    return componentCounts;
}

std::vector<int> AnalyticsEngine::extractTopIndices(const std::vector<std::pair<int, int>>& list, int count) {
    std::vector<int> result;
    for (int i = 0; i < std::min(count, (int)list.size()); ++i) {
        result.push_back(list[i].first);
    }
    return result;
}

void AnalyticsEngine::drawAnalyticsPanelOverlay(const Graph& g) {
    if (!Config::viewerOverlayMode) return;

    // Use a copy for analysis to avoid modifying original graph's cached summary if we don't want to
    Graph temp = g;
    runFullAnalysis(temp);
    const auto& s = temp.summary;

    std::cout << "\n==== ANALYTICS OVERLAY ====\n";
    std::cout << "Nodes: " << s.totalNodes << " | Edges: " << s.totalEdges << "\n";
    std::cout << "Average Degree: " << s.averageDegree << " | Density: " << s.density << "\n";
    std::cout << "Is Connected: " << std::boolalpha << s.isConnected << "\n";
    std::cout << "Isolated Nodes: " << s.isolatedNodeCount << "\n";
    std::cout << "Clustering Coeff: " << s.avgClusteringCoeff << " | Diameter: " << s.diameter << "\n";

    auto printNodeList = [&](const std::string& title, const std::vector<int>& nodesList) {
        std::cout << title;
        if (nodesList.empty()) {
            std::cout << " None\n";
            return;
        }
        for (int idx : nodesList) {
            if (g.nodeExists(idx)) {
                std::cout << " [" << g.nodeMap.at(idx).label << "]";
            } else {
                std::cout << " [#?" << idx << "]";
            }
        }
        std::cout << "\n";
    };

    std::cout << "\n";
    printNodeList("Focused Nodes:          ", s.focusedNodes);
    printNodeList("Top Connected Nodes:    ", s.topConnectedNodes);
    printNodeList("Top Connected Subjects: ", s.topConnectedSubjects);
    printNodeList("Least Connected Nodes:  ", s.leastConnectedNodes);
    printNodeList("Least Connected Subjects:", s.leastConnectedSubjects);

    std::cout << "\nTopic Weights:\n";
    for (const auto& [i, w] : s.topicWeights) {
        if (g.nodeExists(i)) {
            std::cout << "  [" << g.nodeMap.at(i).label << "]: " << w << "\n";
        }
    }

    std::cout << "\nPerformance:\n";
    std::cout << "Load Time: " << s.timeToLoadMs << " ms\n";
    std::cout << "Render Time: " << s.timeToRenderMs << " ms\n";
    std::cout << "=================================\n";
}
