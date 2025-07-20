// analysis_logic.cpp
#include "analysis_logic.h"
#include <iostream>

// Analytics Overlay
void drawAnalyticsPanelOverlay(const Graph& g) {
    if (!Config::viewerOverlayMode) return;

    Graph copy = g;
    copy.updateSummary();
    const auto& s = copy.summary;

    std::cout << "\n==== FULL ANALYTICS OVERLAY ====\n";
    std::cout << "Nodes: " << s.totalNodes << " | Edges: " << s.totalEdges << "\n";
    std::cout << "Average Degree: " << s.averageDegree << "\n";
    std::cout << "Is Connected: " << std::boolalpha << s.isConnected << "\n";
    std::cout << "Isolated Nodes: " << s.isolatedNodeCount << "\n";

    std::cout << "\nFocused Nodes: ";
    for (int idx : s.focusedNodes) std::cout << "#" << idx << " ";
    std::cout << "\n\nTopic Weights:\n";
    for (const auto& [i, w] : s.topicWeights) std::cout << "#" << i << ": " << w << "\n";

    std::cout << "\nTop Connected Nodes: ";
    for (int i : s.topConnectedNodes) std::cout << "#" << i << " ";

    std::cout << "\nTop Connected Subjects: ";
    for (int i : s.topConnectedSubjects) std::cout << "#" << i << " ";

    std::cout << "\nLeast Connected Nodes: ";
    for (int i : s.leastConnectedNodes) std::cout << "#" << i << " ";

    std::cout << "\nLeast Connected Subjects: ";
    for (int i : s.leastConnectedSubjects) std::cout << "#" << i << " ";

    std::cout << "\n\nPerformance:\n";
    std::cout << "Load Time: " << s.timeToLoadMs << " ms\n";
    std::cout << "Render Time: " << s.timeToRenderMs << " ms\n";
    std::cout << "=================================\n";
    g.pause();  // <-- wait for keypress before continuing
}

