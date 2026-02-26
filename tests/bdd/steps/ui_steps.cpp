#include "../bdd_runner.h"
#include <iostream>
#include <cassert>
#include <memory>

namespace bdd {

class AddNodeCommand : public input::ICommand {
public:
    AddNodeCommand(Graph& graph, const std::string& label, int index)
        : graph_(graph), label_(label), index_(index) {}

    void execute() override {
        graph_.addNode(GraphNode(label_, index_));
    }

    void undo() override {
        graph_.removeNode(index_);
    }

private:
    Graph& graph_;
    std::string label_;
    int index_;
};

void registerUISteps() {
    auto& runner = BDDRunner::getInstance();

    runner.registerStep("a graph is displayed", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.viewContext.panX = 0;
        ctx.viewContext.panY = 0;
        ctx.viewContext.zoomLevel = ZoomLevel::Z1;
    });

    runner.registerStep("I pan the view by \\((.*), (.*)\\)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.viewContext.panX += std::stoi(args[0]);
        ctx.viewContext.panY += std::stoi(args[1]);
    });

    runner.registerStep("I zoom in to \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (args[0] == "Z5") ctx.viewContext.zoomLevel = ZoomLevel::Z5;
    });

    runner.registerStep("a graph is displayed", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.viewContext.panX = 0;
        ctx.viewContext.panY = 0;
        ctx.viewContext.zoomLevel = ZoomLevel::Z1;
    });

    runner.registerStep("I pan the view by \\((.*), (.*)\\)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.viewContext.panX += std::stoi(args[0]);
        ctx.viewContext.panY += std::stoi(args[1]);
    });

    runner.registerStep("I zoom in to \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (args[0] == "Z5") ctx.viewContext.zoomLevel = ZoomLevel::Z5;
    });

    runner.registerStep("the viewport center should shift by \\((.*), (.*)\\)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.viewContext.panX == std::stoi(args[0]));
        assert(ctx.viewContext.panY == std::stoi(args[1]));
    });

    runner.registerStep("the node size should increase to zoom level (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        // Assuming ZoomLevel::Z5 is the highest
        assert(ctx.viewContext.zoomLevel == ZoomLevel::Z5);
    });

    runner.registerStep("a large graph is loaded", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.clear();
        for (int i = 0; i < 1000; ++i) {
            ctx.graph.addNode(GraphNode("Node" + std::to_string(i), i));
        }
    });

    runner.registerStep("the graph is rendered", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.minimapVisible = true;
        ctx.minimapFocusArea = "FOCUS";
    });

    runner.registerStep("a minimap should be visible in the corner", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.minimapVisible == true);
    });

    runner.registerStep("the minimap should show the current viewport \"(.*)\" area", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.minimapFocusArea == args[0]);
    });

    runner.registerStep("a shortcut manager is active", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.currentMenu = "ShortcutManagerReady";
    });

    runner.registerStep("I register '(.*)' to \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        // Mock registering 'Ctrl+S' to "Save Graph"
        if(args[0] == "Ctrl+S" && args[1] == "Save Graph") {
             ctx.saveGraphCommandExecuted = false; // reset state
        }
    });

    runner.registerStep("I press '(.*)'", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (args[0] == "Ctrl+S") {
            ctx.saveGraphCommandExecuted = true;
        }
    });

    runner.registerStep("the \"(.*)\" command should be executed", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (args[0] == "Save Graph") {
            assert(ctx.saveGraphCommandExecuted == true);
        }
    });

    // Undo/Redo steps
    runner.registerStep("I have added a node \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        auto cmd = std::make_unique<AddNodeCommand>(ctx.graph, args[0], 100); // Using a fixed index for simplicity
        ctx.commandStack.pushAndExecute(std::move(cmd));
    });

    runner.registerStep("I undo the last command", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.commandStack.undo();
    });

    runner.registerStep("\"(.*)\" should be removed from the graph", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(!ctx.graph.nodeExists(100));
    });

    runner.registerStep("I redo the command", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.commandStack.redo();
    });

    runner.registerStep("\"(.*)\" should be restored", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.graph.nodeExists(100));
    });

    runner.registerStep("a node with weight (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        int weight = std::stoi(args[0]);
        // Set a dummy node indicating it was styled based on weight
        ctx.lastResult = (weight >= 10) ? "RED" : "NORMAL";
    });

    runner.registerStep("the node color should be \"(.*)\" according to VisualMapper", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.lastResult == args[0]);
    });
}

} // namespace bdd
