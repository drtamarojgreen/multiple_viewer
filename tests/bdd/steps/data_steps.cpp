#include "../bdd_runner.h"
#include "io/io_manager.h"
#include "../../src/file_logic.h"
#include "analytics/analytics_engine_ext.h"
#include "viewer_logic.h"
#include <iostream>
#include <fstream>

namespace bdd {

void registerDataSteps() {
    auto& runner = BDDRunner::getInstance();

    runner.registerStep("a file \"(.*)\" in JSON format", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::ofstream out(args[0]);
        out << "{\n";
        out << "  \"nodes\": [\n";
        for(int i=0; i<50; ++i) {
            out << "    {\"label\": \"NODE\", \"index\": " << i << "}" << (i == 49 ? "" : ",") << "\n";
        }
        out << "  ],\n";
        out << "  \"edges\": [\n";
        for(int i=0; i<120; ++i) {
            int src = i % 50;
            int offset = (i / 50) + 1;
            int dst = (src + offset) % 50;
            out << "    {\"source\": " << src << ", \"target\": " << dst << "}" << (i == 119 ? "" : ",") << "\n";
        }
        out << "  ]\n";
        out << "}\n";
        out.close();

        std::ifstream verify(args[0]);
        EXPECT(verify.is_open(), ctx, "Failed to create JSON file " + args[0]);
    });

    runner.registerStep("I load the graph from \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        bool ok = false;
        if (args[0].find(".json") != std::string::npos) {
             ok = io::IOManager::loadJSON(ctx.graph, args[0]);
        } else {
             ok = loadGraphFromCSV(ctx.graph, args[0]);
        }
        EXPECT(ok, ctx, "Failed to load graph from " + args[0]);
        ctx.lastResult = std::to_string(ctx.graph.nodes.size());
    });

    runner.registerStep("I load the graph from \"(.*)\" in JSON format", [](BDDContext& ctx, const std::vector<std::string>& args) {
        bool ok = io::IOManager::loadJSON(ctx.graph, args[0]);
        EXPECT(ok, ctx, "Failed to load JSON graph from " + args[0]);
        ctx.lastResult = std::to_string(ctx.graph.nodes.size());
    });

    runner.registerStep("I save the graph to \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        bool ok = saveGraphToCSV(ctx.graph, args[0]);
        EXPECT(ok, ctx, "Failed to save CSV graph to " + args[0]);
    });

    runner.registerStep("I save the graph to \"(.*)\" in JSON format", [](BDDContext& ctx, const std::vector<std::string>& args) {
        bool ok = io::IOManager::saveJSON(ctx.graph, args[0]);
        EXPECT(ok, ctx, "Failed to save JSON graph to " + args[0]);
    });

    runner.registerStep("I clear the current graph", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.clear();
    });

    runner.registerStep("a populated graph with nodes and edges", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.clear();
        ctx.graph.addNode(GraphNode("A", 0, {}, 1, 0));
        ctx.graph.addNode(GraphNode("B", 1, {}, 1, 0));
        ctx.graph.addEdge(0, 1);
    });

    runner.registerStep("the graph should contain \"(.*)\" nodes and \"(.*)\" edges", [](BDDContext& ctx, const std::vector<std::string>& args) {
        EXPECT(ctx.graph.nodes.size() == std::stoul(args[0]), ctx, "Node count mismatch");
        EXPECT(ctx.graph.edgeCount() == std::stoul(args[1]), ctx, "Edge count mismatch");
    });

    runner.registerStep("I export the graph as SVG to \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        io::IOManager::exportSVG(ctx.graph, args[0]);
        ctx.svgExported = true;
        ctx.lastResult = "exported";
    });

    runner.registerStep("the file \"(.*)\" should be created and contain \"<svg\" tags", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::ifstream in(args[0]);
        EXPECT(in.is_open(), ctx, "SVG file " + args[0] + " was not created");
        std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        EXPECT(content.find("<svg") != std::string::npos, ctx, "SVG tags missing in " + args[0]);
    });

    runner.registerStep("a graph with three distinct clusters", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.clear();
        for(int c=0; c<3; ++c) {
            for(int i=0; i<3; ++i) {
                int id = c*10 + i;
                ctx.graph.addNode(GraphNode("N", id, {}, 1, c));
                for(int j=0; j<i; ++j) ctx.graph.addEdge(id, c*10 + j);
            }
        }
    });

    runner.registerStep("I run community detection", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.updateSummary();
        auto communities = analytics::AnalyticsEngine::detectCommunities(ctx.graph);
        ctx.lastResult = std::to_string(communities.size());
    });

    runner.registerStep("it should identify \"(.*)\" communities", [](BDDContext& ctx, const std::vector<std::string>& args) {
        EXPECT(ctx.lastResult == args[0], ctx, "Community count mismatch");
    });

    runner.registerStep("the file \"(.*)\" should be created and contain \"(.*)\" tags", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::ifstream in(args[0]);
        EXPECT(in.is_open(), ctx, "File " + args[0] + " was not created");
        std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        EXPECT(content.find(args[1]) != std::string::npos, ctx, "Tag missing");
    });

    runner.registerStep("a star-topology graph", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.clear();
        ctx.graph.addNode(GraphNode("Center", 0, {}, 1, 0));
        for(int i=1; i<=5; ++i) {
            ctx.graph.addNode(GraphNode("Leaf", i, {}, 1, 0));
            ctx.graph.addEdge(0, i);
        }
    });

    runner.registerStep("a populated graph", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.clear();
        ctx.graph.addNode(GraphNode("N1", 0, {}, 1, 0));
        ctx.graph.addNode(GraphNode("N2", 1, {}, 1, 0));
        ctx.graph.addEdge(0, 1);
    });

    runner.registerStep("I compute betweenness centrality", [](BDDContext& ctx, const std::vector<std::string>& args) {
        auto metrics = analytics::AnalyticsEngine::computeCentrality(ctx.graph);
        float maxScore = -1.0f;
        int maxNode = -1;
        for(const auto& [id, score] : metrics.betweennessCentrality) {
            if(score > maxScore) { maxScore = score; maxNode = id; }
        }
        ctx.lastResult = std::to_string(maxNode);
    });

    runner.registerStep("the center node should have the \"(.*)\" centrality score", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if(args[0] == "HIGHEST") EXPECT(ctx.lastResult == "0", ctx, "Centrality mismatch");
    });
}

} // namespace bdd
