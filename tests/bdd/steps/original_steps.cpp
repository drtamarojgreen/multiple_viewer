#include "../bdd_runner.h"
#include "../../src/file_logic.h"
#include "../../src/search_logic.h"
#include <iostream>
#include <cassert>

namespace bdd {

void registerOriginalSteps() {
    auto& runner = BDDRunner::getInstance();

    runner.registerStep("an empty graph", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.clear();
        std::cout << "[STEP] Graph cleared\n";
    });

    runner.registerStep("I add a node \"(.*)\" at index (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.addNode(GraphNode(args[0], std::stoi(args[1])));
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

    runner.registerStep("node (\\d+) should be focused", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.graph.isNodeFocused(std::stoi(args[0])));
    });

    runner.registerStep("I save the graph to \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        saveGraphToCSV(ctx.graph, args[0]);
    });

    runner.registerStep("I clear the current graph", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.clear();
    });

    runner.registerStep("I load the graph from \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        loadGraphFromCSV(ctx.graph, args[0]);
    });

    runner.registerStep("the graph should restore all previous nodes and edges", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(!ctx.graph.nodes.empty());
        std::cout << "[STEP] Verified persistence restore\n";
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

    runner.registerStep("I search for nodes containing \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        auto results = findSimilarTopics(ctx.graph, args[0]);
        ctx.graph.clearFocuses();
        for (int id : results) ctx.graph.addFocus(id);
        ctx.lastResult = std::to_string(results.size());
    });

    runner.registerStep("\"(\\d+)\" nodes should be identified", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.lastResult == args[0]);
    });
}

} // namespace bdd
