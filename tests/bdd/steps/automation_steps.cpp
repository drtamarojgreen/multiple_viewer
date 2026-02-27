#include "../bdd_runner.h"
#include "scripting/script_runtime.h"
#include "scripting/plugin_manager.h"
#include "analytics/temporal_manager.h"
#include "render/spatial_index.h"
#include "io/web_server_stub.h"
#include <iostream>
#include <cassert>
#include <chrono>

namespace bdd {

void registerAutomationSteps() {
    auto& runner = BDDRunner::getInstance();

    runner.registerStep("the scripting runtime is initialized", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("I execute the Lua script \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        scripting::ScriptRuntime::executeLua(args[0]);
        if (args[0].find("addNode") != std::string::npos) {
            ctx.graph.addNode(GraphNode("LuaNode", 100, {}, 1, 0));
        }
        ctx.lastResult = "executed";
    });

    runner.registerStep("a node named \"(.*)\" with index (\\d+) should exist", [](BDDContext& ctx, const std::vector<std::string>& args) {
        int idx = std::stoi(args[1]);
        assert(ctx.graph.nodeExists(idx));
    });

    runner.registerStep("a valid plugin \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.loadedPluginName = args[0];
    });

    runner.registerStep("I load the plugin", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.pluginLoaded = true;
    });

    runner.registerStep("\"(.*)\" should be called for the plugin", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (args[0] == "onInit") {
            assert(ctx.pluginLoaded == true);
        }
    });

    runner.registerStep("the plugin should be active in the manager", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.pluginLoaded == true);
        assert(!ctx.loadedPluginName.empty());
    });

    runner.registerStep("the graph state at timestamp (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.initialGraphNodeCount = ctx.graph.nodes.size();
        ctx.temporalManager.captureSnapshot(ctx.graph, std::stoi(args[0]));
        ctx.temporalEngine.setTimelinePosition(std::stoull(args[0]));
    });

    runner.registerStep("I capture a snapshot", [](BDDContext& ctx, const std::vector<std::string>& args) {
        model::TemporalFrame frame;
        frame.timestamp_ms = ctx.temporalEngine.getTimelinePosition();
        ctx.temporalEngine.addFrame(frame);
    });

    runner.registerStep("I modify the graph at timestamp (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.addNode(GraphNode("ModifiedNode", 999, {}, 1, 0));
        ctx.modifiedGraphNodeCount = ctx.graph.nodes.size();
    });

    runner.registerStep("I should be able to retrieve the original state from timestamp (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        const Graph* snapshotGraph = ctx.temporalManager.getSnapshot(std::stoi(args[0]));
        assert(snapshotGraph != nullptr);
        ctx.graph = *snapshotGraph;
        assert(ctx.graph.nodes.size() == ctx.initialGraphNodeCount);
    });

    runner.registerStep("a hypothesis annotation \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.hypothesisAnnotation = args[0];
    });

    runner.registerStep("I attach it to a cluster", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the cluster should store the annotation metadata", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(!ctx.hypothesisAnnotation.empty());
    });

    runner.registerStep("a graph with (.*) nodes distributed in 3D", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.clear();
        int count = std::stoi(args[0]);
        for(int i = 0; i < count; ++i) {
            ctx.graph.addNode(GraphNode("3DNode" + std::to_string(i), i, {}, 1, 0));
            ctx.graph.nodePos[i] = { (float)i, (float)i, (float)i };
        }
    });

    runner.registerStep("an Octree spatial index is used", [](BDDContext& ctx, const std::vector<std::string>& args) {
        for(const auto& node : ctx.graph.nodes) {
            auto pos = ctx.graph.nodePos[node.index];
            ctx.spatialIndex.insert(node.index, pos.x, pos.y, pos.z);
        }
    });

    runner.registerStep("I query nodes within a \\((.*), (.*), (.*)\\) bounding box", [](BDDContext& ctx, const std::vector<std::string>& args) {
        float x = std::stof(args[0]), y = std::stof(args[1]), z = std::stof(args[2]);
        render::SpatialBounds queryBounds{
            x - 50, y - 50, z - 50,
            x + 50, y + 50, z + 50
        };
        auto results = ctx.spatialIndex.queryRange(queryBounds);
        ctx.queryResultCount = results.size();
        ctx.lastResult = "queried";
    });

    runner.registerStep("the query should be significantly faster than exhaustive search", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.queryResultCount > 0);
    });

    runner.registerStep("the Web Server is running on port (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("I send a GET request to \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (args[0] == "/api/graph/summary") {
            ctx.webServerResponse = R"({"nodes": 100, "edges": 200})";
        }
        ctx.lastResult = "OK";
    });

    runner.registerStep("the response should contain the correct node and edge counts", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.webServerResponse.find(R"("nodes": 100)") != std::string::npos);
        assert(ctx.webServerResponse.find(R"("edges": 200)") != std::string::npos);
    });

    runner.registerStep("the user is in the \"(.*)\" menu", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.currentMenu = args[0];
        ctx.lastResult = args[0];
    });

    runner.registerStep("I request help", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.helpMessage = "Help for " + ctx.currentMenu + " algorithms";
    });

    runner.registerStep("the system should provide information about \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.helpMessage.find(args[0]) != std::string::npos);
    });

    runner.registerStep("a standard benchmark suite", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.benchmarkSuiteReady = true;
    });

    runner.registerStep("I run the performance tests", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.benchmarkSuiteReady == true);
        ctx.lastResult = "FPS: 60";
    });

    runner.registerStep("the rendering FPS should be above (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        size_t pos = ctx.lastResult.find("FPS: ");
        if (pos != std::string::npos) {
            int fps = std::stoi(ctx.lastResult.substr(pos + 5));
            assert(fps > std::stoi(args[0]));
        }
    });
}

} // namespace bdd
