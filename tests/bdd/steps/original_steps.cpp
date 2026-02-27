#include "../bdd_runner.h"
#include "file_logic.h"
#include "search_logic.h"
#include <iostream>
#include <cassert>

namespace bdd {

void registerOriginalSteps() {
    auto& runner = BDDRunner::getInstance();

    runner.registerStep("an empty graph", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.clear();
        std::cout << "[STEP] Graph cleared\n";
    });

    runner.registerStep("a graph with node (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.clear();
        ctx.graph.addNode(GraphNode("", std::stoi(args[0]), {}, 1, 0));
    });

    runner.registerStep("I add a node \"(.*)\" at index (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.addNode(GraphNode(args[0], std::stoi(args[1]), {}, 1, 0));
        std::cout << "[STEP] Added node " << args[0] << "\n";
    });

    runner.registerStep("I add an edge between (\\d+) and (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.addEdge(std::stoi(args[0]), std::stoi(args[1]));
        std::cout << "[STEP] Added edge " << args[0] << "-" << args[1] << "\n";
    });

    runner.registerStep("node (\\d+) should exist", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.graph.nodeExists(std::stoi(args[0])));
        std::cout << "[STEP] Verified node " << args[0] << " exists\n";
    });

    runner.registerStep("node (\\d+) should be connected to node (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        int u = std::stoi(args[0]);
        int v = std::stoi(args[1]);
        const auto& neighbors = ctx.graph.nodeMap.at(u).neighbors;
        bool connected = false;
        for (int n : neighbors) if (n == v) connected = true;
        assert(connected);
        std::cout << "[STEP] Verified connection " << u << "->" << v << "\n";
    });

    runner.registerStep("I add focus to node (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.addFocus(std::stoi(args[0]));
    });

    runner.registerStep("I remove focus from node (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.removeFocus(std::stoi(args[0]));
    });

    runner.registerStep("node (\\d+) should be focused", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.graph.isNodeFocused(std::stoi(args[0])));
    });

    runner.registerStep("node (\\d+) should not be focused", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(!ctx.graph.isNodeFocused(std::stoi(args[0])));
    });

    runner.registerStep("I cycle focus", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.cycleFocus();
    });

    runner.registerStep("a node should be focused", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.graph.focusedNodeIndex != -1 || !ctx.graph.focusedNodeIndices.empty());
    });

    runner.registerStep("I save the graph to \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        saveGraphToCSV((ctx.graph), args[0]);
    });

    runner.registerStep("I clear the current graph", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.clear();
    });

    runner.registerStep("I load the graph from \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        loadGraphFromCSV((ctx.graph), args[0]);
    });

    runner.registerStep("a persistent graph with nodes 1 and 2", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.clear();
        ctx.graph.addNode(GraphNode("A", 1, {}, 1, 0));
        ctx.graph.addNode(GraphNode("B", 2, {}, 1, 0));
        ctx.graph.addEdge(1, 2);
    });

    runner.registerStep("the graph should restore all previous nodes and edges", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(!ctx.graph.nodes.empty());
        assert(ctx.graph.nodeExists(1));
        assert(ctx.graph.nodeExists(2));
        bool found = false;
        for (int n : ctx.graph.nodeMap.at(1).neighbors) if (n == 2) found = true;
        assert(found);
        std::cout << "[STEP] Verified persistence restore\n";
    });

    runner.registerStep("a triangle graph \\(nodes 0, 1, 2 all connected\\)", [](BDDContext& ctx, const std::vector<std::string>& args) {
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
        if (args[0] == "Connected") assert(ctx.graph.isConnected());
    });

    runner.registerStep("the average degree should be \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(std::abs(ctx.graph.computeAvgDegree() - std::stof(args[0])) < 0.01f);
    });

    runner.registerStep("the clustering coefficient should be greater than \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.graph.summary.avgClusteringCoeff > std::stof(args[0]));
    });

    runner.registerStep("a graph with nodes \"([^\"]*)\", \"([^\"]*)\", \"([^\"]*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.addNode(GraphNode(args[0], 0, {}, 1, 0));
        ctx.graph.addNode(GraphNode(args[1], 1, {}, 1, 0));
        ctx.graph.addNode(GraphNode(args[2], 2, {}, 1, 0));
    });

    runner.registerStep("I search for nodes containing \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        auto results = findSimilarTopics((ctx.graph), args[0]);
        ctx.graph.clearFocuses();
        for (int id : results) ctx.graph.addFocus(id);
        ctx.lastResult = std::to_string(results.size());
    });

    runner.registerStep("\"(\\d+)\" nodes should be identified", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (ctx.lastResult != args[0]) {
             std::cerr << "[BDD FAIL] Expected " << args[0] << " search results, got " << ctx.lastResult << std::endl;
        }
        assert(ctx.lastResult == args[0]);
    });

    runner.registerStep("they should be added to the focus set", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(!ctx.graph.focusedNodeIndices.empty());
    });
}

} // namespace bdd
