#include "../bdd_runner.h"
#include "render/visual_mapper.h"
#include "input/shortcut_manager.h"
#include "input/command_stack.h"
#include <iostream>

namespace bdd {

void registerRenderingSteps() {
    auto& runner = BDDRunner::getInstance();

    runner.registerStep("a graph is displayed", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("I pan the view by \\((.*), (.*)\\)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ViewContext view;
        view.pan(std::stoi(args[0]), std::stoi(args[1]));
        ctx.lastResult = "panned";
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
}

} // namespace bdd
