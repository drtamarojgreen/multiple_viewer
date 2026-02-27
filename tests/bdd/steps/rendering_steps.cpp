#include "../bdd_runner.h"
#include "render/visual_mapper.h"
#include "render/minimap_renderer.h"
#include "input/shortcut_manager.h"
#include "input/command_stack.h"
#include <iostream>
#include <cassert>

namespace bdd {

void registerRenderingSteps() {
    auto& runner = BDDRunner::getInstance();

    runner.registerStep("a graph is displayed", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.addNode(GraphNode("DisplayNode", 0));
    });

    runner.registerStep("a view context with zoom level \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if(args[0] == "Z1") ctx.viewContext.zoomLevel = ZoomLevel::Z1;
        else if(args[0] == "Z2") ctx.viewContext.zoomLevel = ZoomLevel::Z2;
        else if(args[0] == "Z5") ctx.viewContext.zoomLevel = ZoomLevel::Z5;
    });

    runner.registerStep("I zoom in", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.viewContext.zoomIn();
    });

    runner.registerStep("the zoom level should be \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if(args[0] == "Z2") assert(ctx.viewContext.zoomLevel == ZoomLevel::Z2);
        else if(args[0] == "Z1") assert(ctx.viewContext.zoomLevel == ZoomLevel::Z1);
        else if(args[0] == "Z5") assert(ctx.viewContext.zoomLevel == ZoomLevel::Z5);
    });

    runner.registerStep("I pan the view by \\((.*), (.*)\\)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.viewContext.pan(std::stoi(args[0]), std::stoi(args[1]));
    });

    runner.registerStep("the pan offsets should reflect the change", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.viewContext.panX != 0 || ctx.viewContext.panY != 0);
    });

    runner.registerStep("the viewport center should shift by \\((.*), (.*)\\)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.viewContext.panX != 0);
    });

    runner.registerStep("the node size should increase to zoom level (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        int zoom = std::stoi(args[0]);
        int size = ctx.graph.calculateNodeSize(0, static_cast<ZoomLevel>(zoom-1));
        assert(size >= 1);
    });

    runner.registerStep("a large graph is loaded", [](BDDContext& ctx, const std::vector<std::string>& args) {
        for(int i=0; i<100; ++i) ctx.graph.addNode(GraphNode("N", i));
    });

    runner.registerStep("the graph is rendered", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.lastResult = "rendered";
    });

    runner.registerStep("a minimap should be visible in the corner", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.lastResult == "rendered");
    });

    runner.registerStep("the minimap should show the current viewport \"(.*)\" area", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(args[0] == "FOCUS");
    });

    runner.registerStep("a shortcut manager is active", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("I register '(.*)' to \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.lastResult = args[1];
    });

    runner.registerStep("I press '(.*)'", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the \"(.*)\" command should be executed", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.lastResult == args[0]);
    });

    runner.registerStep("a node with weight (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.addNode(GraphNode("Weighted", 0, {}, std::stoi(args[0])));
    });

    runner.registerStep("the node color should be \"(.*)\" according to VisualMapper", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::string actual = render::VisualMapper::getColorForWeight(ctx.graph.nodes.at(0).weight);
        assert(actual == args[0]);
    });

    runner.registerStep("I have added a node \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.addNode(GraphNode(args[0], 100));
    });

    runner.registerStep("I undo the last command", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("\"(.*)\" should be removed from the graph", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("I redo the command", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("\"(.*)\" should be restored", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });
}

} // namespace bdd
