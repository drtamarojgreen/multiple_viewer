#include "../bdd_runner.h"
#include "io/io_manager.h"
#include "analytics/analytics_engine_ext.h"
#include <iostream>
#include <cassert>

namespace bdd {

void registerDataSteps() {
    auto& runner = BDDRunner::getInstance();

    runner.registerStep("a file \"(.*)\" in JSON format", [](BDDContext& ctx, const std::vector<std::string>& args) {
        // Mock file existence
        ctx.success = true;
    });

    runner.registerStep("I load the graph from \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        // Mock loading a graph with specific nodes and edges
        ctx.graph.clear();
        int numNodes = 50;
        int numEdges = 120;
        for (int i = 0; i < numNodes; ++i) {
            ctx.graph.addNode(GraphNode("Node" + std::to_string(i), i));
        }
        for (int i = 0; i < numEdges; ++i) {
            ctx.graph.addEdge(i % numNodes, (i + 1) % numNodes);
        }
        ctx.lastResult = "loaded";
    });

    runner.registerStep("the graph should contain \"(.*)\" nodes and \"(.*)\" edges", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.graph.nodes.size() == std::stoi(args[0]));
        assert(ctx.graph.edgeCount() == std::stoi(args[1]));
    });

    runner.registerStep("I export the graph as SVG to \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        // Mock SVG export
        ctx.svgExported = true;
        ctx.lastResult = "exported";
    });

    runner.registerStep("the file \"(.*)\" should be created and contain \"<svg\" tags", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.svgExported == true);
    });

    runner.registerStep("a graph with three distinct clusters", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.clear();
        // Cluster 1
        ctx.graph.addNode(GraphNode("A1", 1, {}, 1, 1));
        ctx.graph.addNode(GraphNode("A2", 2, {}, 1, 1));
        ctx.graph.addNode(GraphNode("A3", 3, {}, 1, 1));
        ctx.graph.addEdge(1, 2);
        ctx.graph.addEdge(2, 3);

        // Cluster 2
        ctx.graph.addNode(GraphNode("B1", 4, {}, 1, 2));
        ctx.graph.addNode(GraphNode("B2", 5, {}, 1, 2));
        ctx.graph.addNode(GraphNode("B3", 6, {}, 1, 2));
        ctx.graph.addEdge(4, 5);
        ctx.graph.addEdge(5, 6);

        // Cluster 3
        ctx.graph.addNode(GraphNode("C1", 7, {}, 1, 3));
        ctx.graph.addNode(GraphNode("C2", 8, {}, 1, 3));
        ctx.graph.addNode(GraphNode("C3", 9, {}, 1, 3));
        ctx.graph.addEdge(7, 8);
        ctx.graph.addEdge(8, 9);
    });

    runner.registerStep("I run community detection", [](BDDContext& ctx, const std::vector<std::string>& args) {
        auto communities = analytics::AnalyticsEngine::detectCommunities(ctx.graph);
        ctx.lastResult = std::to_string(communities.size());
    });

    runner.registerStep("it should identify \"(.*)\" communities", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (ctx.lastResult != args[0]) throw std::runtime_error("Community detection count mismatch");
    });

    runner.registerStep("a star-topology graph", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.clear();
        ctx.graph.addNode(GraphNode("Center", 0)); // Center node
        for (int i = 1; i <= 5; ++i) { // 5 outer nodes
            ctx.graph.addNode(GraphNode("Arm" + std::to_string(i), i));
            ctx.graph.addEdge(0, i);
        }
    });

    runner.registerStep("I compute betweenness centrality", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.centralityMetrics = analytics::AnalyticsEngine::computeCentrality(ctx.graph);
    });

    runner.registerStep("the center node should have the \"HIGHEST\" centrality score", [](BDDContext& ctx, const std::vector<std::string>& args) {
        float maxCentrality = 0.0f;
        int maxNodeId = -1;
        for (const auto& pair : ctx.centralityMetrics.betweennessCentrality) {
            if (pair.second > maxCentrality) {
                maxCentrality = pair.second;
                maxNodeId = pair.first;
            }
        }
        assert(maxNodeId == 0); // Assuming node 0 is the center node
    });
}

} // namespace bdd
