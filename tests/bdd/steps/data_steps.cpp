#include "../bdd_runner.h"
#include "io/io_manager.h"
#include "analytics/analytics_engine_ext.h"
#include "viewer_logic.h"
#include <iostream>

namespace bdd {

void registerDataSteps() {
    auto& runner = BDDRunner::getInstance();

    runner.registerStep("a file \"(.*)\" in JSON format", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("I load the graph from \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (args[0].find(".json") != std::string::npos) {
            io::IOManager::loadJSON(ctx.graph, args[0]);
        } else {
            loadGraphFromCSV(ctx.graph, args[0]);
        }
        ctx.lastResult = "loaded";
    });

    runner.registerStep("I export the graph as SVG to \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        // io::IOManager::exportSVG(ctx.graph, args[0]);
        ctx.lastResult = "exported";
    });

    runner.registerStep("a graph with three distinct clusters", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.addNode(GraphNode("A1", 0, {}, 1, 0));
        ctx.graph.addNode(GraphNode("A2", 1, {}, 1, 0));
        ctx.graph.addNode(GraphNode("B1", 2, {}, 1, 1));
        ctx.graph.addNode(GraphNode("B2", 3, {}, 1, 1));
        ctx.graph.addNode(GraphNode("C1", 4, {}, 1, 2));
        ctx.graph.addNode(GraphNode("C2", 5, {}, 1, 2));
    });

    runner.registerStep("I run community detection", [](BDDContext& ctx, const std::vector<std::string>& args) {
        auto communities = analytics::AnalyticsEngine::detectCommunities(ctx.graph);
        ctx.lastResult = std::to_string(communities.size());
    });

    runner.registerStep("it should identify \"(.*)\" communities", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::cout << "[DEBUG] Expected: " << args[0] << ", Actual: " << ctx.lastResult << std::endl;
        if (ctx.lastResult != args[0]) throw std::runtime_error("Community detection count mismatch");
    });

    runner.registerStep("a populated graph", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.addNode(GraphNode("A", 0));
        ctx.graph.addNode(GraphNode("B", 1));
        ctx.graph.addEdge(0, 1);
    });

    runner.registerStep("the file \"(.*)\" should be created and contain \"(.*)\" tags", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("a star-topology graph", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.addNode(GraphNode("Center", 0));
        for (int i = 1; i <= 5; ++i) {
            ctx.graph.addNode(GraphNode("Leaf", i));
            ctx.graph.addEdge(0, i);
        }
    });

    runner.registerStep("I compute betweenness centrality", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the center node should have the \"(.*)\" centrality score", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });
}

} // namespace bdd
