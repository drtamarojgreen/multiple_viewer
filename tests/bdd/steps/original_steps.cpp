#include "../bdd_runner.h"
#include "file_logic.h"
#include "search_logic.h"
#include "io/io_manager.h"
#include <iostream>
#include <cmath>

namespace bdd {

void registerOriginalSteps() {
    auto& runner = BDDRunner::getInstance();

    runner.registerStep("an empty graph", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.clear();
    });

    runner.registerStep("a graph with node (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.clear();
        ctx.graph.addNode(GraphNode("", std::stoi(args[0]), {}, 1, 0));
    });

    runner.registerStep("I add a node \"(.*)\" at index (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.addNode(GraphNode(args[0], std::stoi(args[1]), {}, 1, 0));
    });

    runner.registerStep("I add an edge between (\\d+) and (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.addEdge(std::stoi(args[0]), std::stoi(args[1]));
    });

    runner.registerStep("node (\\d+) should exist", [](BDDContext& ctx, const std::vector<std::string>& args) {
        EXPECT(ctx.graph.nodeExists(std::stoi(args[0])), ctx, "Node missing");
    });

    runner.registerStep("node (\\d+) should have label \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        int id = std::stoi(args[0]);
        std::string expected = args[1];
        EXPECT(ctx.graph.nodeMap.count(id) > 0, ctx, "Node missing");
        if (ctx.graph.nodeMap.count(id) > 0) {
            EXPECT(ctx.graph.nodeMap.at(id).label == expected, ctx, "Label mismatch");
        }
    });

    runner.registerStep("node (\\d+) should be connected to node (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        int u = std::stoi(args[0]);
        int v = std::stoi(args[1]);
        EXPECT(ctx.graph.nodeExists(u), ctx, "Node missing");
        if (ctx.graph.nodeExists(u)) {
            const auto& neighbors = ctx.graph.nodeMap.at(u).neighbors;
            bool connected = false;
            for (int n : neighbors) if (n == v) connected = true;
            EXPECT(connected, ctx, "Not connected");
        }
    });

    runner.registerStep("I add focus to node (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.addFocus(std::stoi(args[0]));
    });

    runner.registerStep("I remove focus from node (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.removeFocus(std::stoi(args[0]));
    });

    runner.registerStep("node (\\d+) should be focused", [](BDDContext& ctx, const std::vector<std::string>& args) {
        EXPECT(ctx.graph.isNodeFocused(std::stoi(args[0])), ctx, "Not focused");
    });

    runner.registerStep("node (\\d+) should not be focused", [](BDDContext& ctx, const std::vector<std::string>& args) {
        EXPECT(!ctx.graph.isNodeFocused(std::stoi(args[0])), ctx, "Should not be focused");
    });

    runner.registerStep("I cycle focus", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.cycleFocus();
    });

    runner.registerStep("a node should be focused", [](BDDContext& ctx, const std::vector<std::string>& args) {
        EXPECT(ctx.graph.focusedNodeIndex != -1 || !ctx.graph.focusedNodeIndices.empty(), ctx, "No focus");
    });

    runner.registerStep("a persistent graph with nodes 1 and 2", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.clear();
        ctx.graph.addNode(GraphNode("A", 1, {}, 1, 0));
        ctx.graph.addNode(GraphNode("B", 2, {}, 1, 0));
        ctx.graph.addEdge(1, 2);
    });

    runner.registerStep("the graph should restore all previous nodes and edges", [](BDDContext& ctx, const std::vector<std::string>& args) {
        EXPECT(ctx.graph.nodeExists(1), ctx, "Node 1 missing");
        EXPECT(ctx.graph.nodeExists(2), ctx, "Node 2 missing");
        if (ctx.graph.nodeExists(1)) {
            bool found = false;
            for (int n : ctx.graph.nodeMap.at(1).neighbors) if (n == 2) found = true;
            EXPECT(found, ctx, "Edge missing");
        }
    });

    runner.registerStep("a triangle graph \\(nodes 0, 1, 2 all connected\\)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.clear();
        ctx.graph.addNode(GraphNode("0", 0, {}, 1, 0));
        ctx.graph.addNode(GraphNode("1", 1, {}, 1, 0));
        ctx.graph.addNode(GraphNode("2", 2, {}, 1, 0));
        ctx.graph.addEdge(0, 1);
        ctx.graph.addEdge(1, 2);
        ctx.graph.addEdge(2, 0);
    });

    runner.registerStep("I run full graph analysis", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.updateSummary();
    });

    runner.registerStep("the graph should be reported as \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (args[0] == "Connected") EXPECT(ctx.graph.isConnected(), ctx, "Disconnected");
    });

    runner.registerStep("the average degree should be \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        EXPECT(std::abs(ctx.graph.computeAvgDegree() - std::stof(args[0])) < 0.01f, ctx, "Avg degree mismatch");
    });

    runner.registerStep("the clustering coefficient should be greater than \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        EXPECT(ctx.graph.summary.avgClusteringCoeff > std::stof(args[0]), ctx, "Clustering low");
    });

    runner.registerStep("a graph with nodes \"([^\"]*)\", \"([^\"]*)\", \"([^\"]*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.clear();
        ctx.graph.addNode(GraphNode(args[0], 0, {}, 1, 0));
        ctx.graph.addNode(GraphNode(args[1], 1, {}, 1, 0));
        ctx.graph.addNode(GraphNode(args[2], 2, {}, 1, 0));
    });

    runner.registerStep("I search for nodes containing \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        auto results = findSimilarTopics((ctx.graph), args[0]);
        ctx.graph.clearFocuses();
        for (int id : results) {
            ctx.graph.addFocus(id);
        }
        ctx.lastResult = std::to_string(results.size());
    });

    runner.registerStep("\"(\\d+)\" nodes should be identified", [](BDDContext& ctx, const std::vector<std::string>& args) {
        EXPECT(ctx.lastResult == args[0], ctx, "Search count mismatch");
    });

    runner.registerStep("they should be added to the focus set", [](BDDContext& ctx, const std::vector<std::string>& args) {
        EXPECT(!ctx.graph.focusedNodeIndices.empty(), ctx, "Empty focus");
    });
}

} // namespace bdd
