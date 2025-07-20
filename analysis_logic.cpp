// analysis_logic.cpp
#include "analysis_logic.h"
#include <iostream>

// Analytics Overlay
void drawAnalyticsPanelOverlay(const Graph& g) {
    if (!Config::viewerOverlayMode) return;

    Graph copy = g;
    copy.updateSummary();
    const auto& s = copy.summary;

    std::cout << "\n==== ANALYTICS OVERLAY ====\n";
    std::cout << "Nodes: " << s.totalNodes << " | Edges: " << s.totalEdges << "\n";
    std::cout << "Average Degree: " << s.averageDegree << "\n";
    std::cout << "Is Connected: " << std::boolalpha << s.isConnected << "\n";
    std::cout << "Isolated Nodes: " << s.isolatedNodeCount << "\n";

    auto printNodeList = [&](const std::string& title, const std::vector<int>& nodes) {
        std::cout << title;
        if (nodes.empty()) {
            std::cout << " None\n";
            return;
        }
        for (int idx : nodes) {
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
    std::cout << "Clustering Coefficient: " << calculateClusteringCoefficient(g) << "\n";
    std::cout << "Average Degree: " << s.averageDegree << "\n";

    std::cout << "\n\nPerformance:\n";
    std::cout << "Load Time: " << s.timeToLoadMs << " ms\n";
    std::cout << "Render Time: " << s.timeToRenderMs << " ms\n";
    std::cout << "=================================\n";
}
