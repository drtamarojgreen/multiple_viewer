// analysis_logic.h
#ifndef ANALYSIS_LOGIC_H
#define ANALYSIS_LOGIC_H

#include "map_logic.h"
#include <vector>
#include <unordered_map>

class AnalyticsEngine {
public:
    // Performs full analysis on the graph and updates its summary
    static void runFullAnalysis(Graph& g);

    // Individual metrics
    static double calculateClusteringCoefficient(const Graph& g);
    static int calculateGraphDiameter(const Graph& g);
    static float calculateDensity(const Graph& g);
    static std::vector<int> findComponents(const Graph& g);
    static std::vector<int> extractTopIndices(const std::vector<std::pair<int, int>>& list, int count);

    // UI/Output
    static void drawAnalyticsPanelOverlay(const Graph& g);
};

#endif // ANALYSIS_LOGIC_H
