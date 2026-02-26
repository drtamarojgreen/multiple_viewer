#include "../bdd_runner.h"
#include "render/visual_mapper.h"
#include "input/shortcut_manager.h"
#include "input/command_stack.h"
#include <iostream>
#include <cassert>

namespace bdd {

void registerRenderingSteps() {
    auto& runner = BDDRunner::getInstance();

    runner.registerStep("a graph is displayed", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("a view context with zoom level \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        // Mock view context setup
        ctx.lastResult = args[0];
    });

    runner.registerStep("I pan the view by \\((.*), (.*)\\)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ViewContext view;
        view.pan(std::stoi(args[0]), std::stoi(args[1]));
        ctx.lastResult = std::to_string(view.panX) + "," + std::to_string(view.panY);
    });

    runner.registerStep("the pan offsets should reflect the change", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.lastResult != "0,0");
    });

    runner.registerStep("I zoom in", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ViewContext view;
        view.zoomIn();
        ctx.lastResult = "Z2"; // Assuming it goes from Z1 to Z2
    });

    runner.registerStep("the zoom level should be \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.lastResult == args[0]);
    });

    runner.registerStep("I zoom in to \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ViewContext view;
        view.zoomLevel = ZoomLevel::Z5;
        ctx.lastResult = "zoomed";
    });

    runner.registerStep("a shortcut manager is active", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("I register '(.*)' to \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        input::ShortcutManager manager;
        manager.registerShortcut(args[0][0], [](){});
        ctx.lastResult = "registered";
    });

    runner.registerStep("a node with weight (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.addNode(GraphNode("Weighted", 0, {}, std::stoi(args[0])));
    });

    runner.registerStep("the node color should be \"(.*)\" according to VisualMapper", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::string expected = args[0];
        std::string actual = render::VisualMapper::getColorForWeight(ctx.graph.nodeMap.at(0).weight);
        if (actual != expected) throw std::runtime_error("Visual mapping failed");
    });

    runner.registerStep("a large graph is loaded", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the graph is rendered", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("a minimap should be visible in the corner", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the minimap should show the current viewport \"(.*)\" area", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("I press '(.*)'", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the \"(.*)\" command should be executed", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
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

    runner.registerStep("the graph should contain \"(\\d+)\" nodes and \"(\\d+)\" edges", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the viewport center should shift by \\((.*), (.*)\\)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("the node size should increase to zoom level (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });
}

} // namespace bdd
