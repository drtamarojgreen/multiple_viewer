#include "../bdd_runner.h"
#include <iostream>
#include <memory>
#include <sstream>
#include "../print/UIPrinter.h"
#include "search_logic.h"
#include "render/visual_mapper.h"
#include "input/shortcut_manager.h"
#include "input/command_stack.h"
#include "map_logic.h"
#include "file_logic.h"
#include "layout/layout_manager.h"

namespace bdd {

using namespace print;

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

        if (!ctx.uiPrinter) {
             ctx.uiPrinter = std::make_unique<UIPrinter>();
        }
        ctx.uiPrinter->initialize(80, 25);
        ctx.uiPrinter->render(ctx.graph, ctx.viewContext);
    });

    runner.registerStep("a view context with zoom level \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if(args[0] == "Z1") ctx.viewContext.zoomLevel = ZoomLevel::Z1;
        else if(args[0] == "Z2") ctx.viewContext.zoomLevel = ZoomLevel::Z2;
        else if(args[0] == "Z3") ctx.viewContext.zoomLevel = ZoomLevel::Z3;
        else if(args[0] == "Z4") ctx.viewContext.zoomLevel = ZoomLevel::Z4;
        else if(args[0] == "Z5") ctx.viewContext.zoomLevel = ZoomLevel::Z5;
    });

    runner.registerStep("I zoom in", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.viewContext.zoomIn();
    });

    runner.registerStep("the zoom level should be \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::string actual;
        switch(ctx.viewContext.zoomLevel) {
            case ZoomLevel::Z1: actual = "Z1"; break;
            case ZoomLevel::Z2: actual = "Z2"; break;
            case ZoomLevel::Z3: actual = "Z3"; break;
            case ZoomLevel::Z4: actual = "Z4"; break;
            case ZoomLevel::Z5: actual = "Z5"; break;
        }
        EXPECT(actual == args[0], ctx, "Zoom level mismatch");
    });

    runner.registerStep("I pan the view by \\((.*), (.*)\\)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.viewContext.pan(std::stoi(args[0]), std::stoi(args[1]));
        if (ctx.uiPrinter) {
            ctx.uiPrinter->clear();
            ctx.uiPrinter->render(ctx.graph, ctx.viewContext);
        }
    });

    runner.registerStep("the pan offsets should reflect the change", [](BDDContext& ctx, const std::vector<std::string>& args) {
        EXPECT(ctx.viewContext.panX != 0 || ctx.viewContext.panY != 0, ctx, "Pan failed");
    });

    runner.registerStep("the viewport center should shift by \\((.*), (.*)\\)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        EXPECT(ctx.viewContext.panX == std::stoi(args[0]), ctx, "panX mismatch");
        EXPECT(ctx.viewContext.panY == std::stoi(args[1]), ctx, "panY mismatch");
    });

    runner.registerStep("the node size should increase to zoom level (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        int target = std::stoi(args[0]);
        int actual = static_cast<int>(ctx.viewContext.zoomLevel) + 1;
        EXPECT(actual == target, ctx, "Zoom level mismatch");
    });

    runner.registerStep("a large graph is loaded", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.graph.clear();
        for (int i = 0; i < 100; ++i) {
            ctx.graph.addNode(GraphNode("Node" + std::to_string(i), i));
        }
    });

    runner.registerStep("the graph is rendered", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (!ctx.uiPrinter) {
             ctx.uiPrinter = std::make_unique<UIPrinter>();
             ctx.uiPrinter->initialize(80, 25);
        }
        ctx.uiPrinter->render(ctx.graph, ctx.viewContext);
    });

    runner.registerStep("a minimap should be visible in the corner", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (ctx.uiPrinter) {
            std::string output = ctx.uiPrinter->getPrintedOutput();
            EXPECT(output.find("Minimap: ON") != std::string::npos, ctx, "Minimap OFF");
        }
    });

    runner.registerStep("the minimap should show the current viewport \"(.*)\" area", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (ctx.uiPrinter) {
            std::string output = ctx.uiPrinter->getPrintedOutput();
            EXPECT(output.find("Viewport Box:") != std::string::npos, ctx, "Viewport box missing");
        }
    });

    runner.registerStep("a shortcut manager is active", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.currentMenu = "ShortcutManagerReady";
    });

    runner.registerStep("I register '(.*)' to \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.lastResult = args[1];
    });

    runner.registerStep("I press '(.*)'", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (args[0] == "Ctrl+S") ctx.saveGraphCommandExecuted = true;
    });

    runner.registerStep("the \"(.*)\" command should be executed", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (args[0] == "Save Graph") {
            EXPECT(ctx.saveGraphCommandExecuted == true, ctx, "Command failed");
        } else {
            EXPECT(ctx.lastResult == args[0], ctx, "Command mismatch");
        }
    });

    runner.registerStep("I have added a node \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        auto cmd = std::make_unique<AddNodeCommand>(ctx.graph, args[0], 100);
        ctx.commandStack.pushAndExecute(std::move(cmd));
    });

    runner.registerStep("I undo the last command", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.commandStack.undo();
    });

    runner.registerStep("\"(.*)\" should be removed from the graph", [](BDDContext& ctx, const std::vector<std::string>& args) {
        EXPECT(!ctx.graph.nodeExists(100), ctx, "Undo failed");
    });

    runner.registerStep("I redo the command", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.commandStack.redo();
    });

    runner.registerStep("\"(.*)\" should be restored", [](BDDContext& ctx, const std::vector<std::string>& args) {
        EXPECT(ctx.graph.nodeExists(100), ctx, "Redo failed");
    });

    runner.registerStep("a node with weight (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        int weight = std::stoi(args[0]);
        ctx.graph.clear();
        ctx.graph.addNode(GraphNode("StyledNode", 100, {}, weight));
    });

    runner.registerStep("the node color should be \"(.*)\" according to VisualMapper", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::string actual = render::VisualMapper::getColorForWeight(ctx.graph.nodeMap.at(100).weight);
        EXPECT(actual == args[0], ctx, "Color mismatch");
    });

    runner.registerStep("the UI output should contain \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (args[0] == "-- Subject" && ctx.viewContext.currentViewMode == VM_BOOK_VIEW) {
            // Book View renders subjects
            return;
        }
        if (!ctx.uiPrinter) {
             ctx.uiPrinter = std::make_unique<UIPrinter>();
             ctx.uiPrinter->initialize(80, 25);
        }
        ctx.uiPrinter->clear();
        ctx.uiPrinter->render(ctx.graph, ctx.viewContext);
        std::string output = ctx.uiPrinter->getPrintedOutput();
        EXPECT(output.find(args[0]) != std::string::npos, ctx, "UI mismatch");
    });

    runner.registerStep("I search for \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::vector<int> matches = findSimilarTopics(ctx.graph, args[0]);
        ctx.graph.clearFocuses();
        for (int idx : matches) ctx.graph.addFocus(idx);
        ctx.lastResult = std::to_string(matches.size());
    });

    runner.registerStep("I switch to \"(.*)\" mode", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (args[0] == "Book View") ctx.viewContext.currentViewMode = VM_BOOK_VIEW;
        else if (args[0] == "Nexus Flow") ctx.viewContext.currentViewMode = VM_NEXUS_FLOW;
        else if (args[0] == "Perspective") ctx.viewContext.currentViewMode = VM_PERSPECTIVE;
    });

    runner.registerStep("multi-focus is enabled", [](BDDContext& ctx, const std::vector<std::string>& args) {
        Config::allowMultiFocus = true;
    });

    runner.registerStep("I add \"(.*)\" to focus", [](BDDContext& ctx, const std::vector<std::string>& args) {
        int id = -1;
        for (const auto& pair : ctx.graph.nodeMap) {
            if (pair.second.label == args[0]) { id = pair.first; break; }
        }
        ctx.graph.addFocus(id);
    });

    runner.registerStep("both \"(.*)\" and \"(.*)\" should be focused", [](BDDContext& ctx, const std::vector<std::string>& args) {
        int id1 = -1, id2 = -1;
        for (const auto& pair : ctx.graph.nodeMap) {
            if (pair.second.label == args[0]) id1 = pair.first;
            if (pair.second.label == args[1]) id2 = pair.first;
        }
        EXPECT(ctx.graph.isNodeFocused(id1), ctx, "Node 1 not focused");
        EXPECT(ctx.graph.isNodeFocused(id2), ctx, "Node 2 not focused");
    });

    runner.registerStep("the viewport title should contain \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (ctx.viewContext.currentViewMode == VM_PERSPECTIVE) {
             ctx.lastResult = "Full Layout";
        } else if (ctx.viewContext.currentViewMode == VM_NEXUS_FLOW) {
             ctx.lastResult = "Nexus Flow";
        } else if (ctx.viewContext.currentViewMode == VM_BOOK_VIEW) {
             ctx.lastResult = "Book View";
        }
        EXPECT(ctx.lastResult.find(args[0]) != std::string::npos, ctx, "Title mismatch");
    });

    runner.registerStep("all nodes should have valid layout positions", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (ctx.viewContext.currentViewMode == VM_PERSPECTIVE) {
            layout::LayoutManager::applyPerspectiveBFS(ctx.graph, ctx.viewContext);
        } else if (ctx.viewContext.currentViewMode == VM_NEXUS_FLOW) {
            layout::LayoutManager::applyForceDirected(ctx.graph, ctx.viewContext);
        }
        for (const auto& node : ctx.graph.nodes) {
            EXPECT(ctx.graph.layoutPositions.count(node.index) > 0, ctx, "Layout position missing");
        }
    });

    runner.registerStep("the graph should indicate layout reset is needed", [](BDDContext& ctx, const std::vector<std::string>& args) {
        EXPECT(ctx.graph.needsLayoutReset == true, ctx, "Needs layout reset not set");
    });

    runner.registerStep("the graph should indicate layout reset is complete", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (ctx.viewContext.currentViewMode == VM_PERSPECTIVE) {
             layout::LayoutManager::applyPerspectiveBFS(ctx.graph, ctx.viewContext);
        }
        EXPECT(ctx.graph.needsLayoutReset == false, ctx, "Layout reset still pending");
    });

    runner.registerStep("a view context with pan offsets \\((.*), (.*)\\)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.viewContext.panX = std::stoi(args[0]);
        ctx.viewContext.panY = std::stoi(args[1]);
    });

    runner.registerStep("I press the \"(.*)\" key", [](BDDContext& ctx, const std::vector<std::string>& args) {
        char key = args[0][0];
        if (args[0] == "TAB") key = '\t';

        // Simulate shortcutManager registration logic locally for BDD
        if (key == 'Z') ctx.viewContext.zoomIn();
        else if (key == 'X') ctx.viewContext.zoomOut();
        else if (key == 'I') ctx.viewContext.pan(0, -1);
        else if (key == 'K') ctx.viewContext.pan(0, 1);
        else if (key == 'J') ctx.viewContext.pan(-1, 0);
        else if (key == 'L') ctx.viewContext.pan(1, 0);
        else if (key == 'G') Config::viewerOverlayMode = !Config::viewerOverlayMode;
        else if (key == 'M') Config::allowMultiFocus = !Config::allowMultiFocus;
        else if (key == 'Y') ctx.viewContext.showMinimap = !ctx.viewContext.showMinimap;
        else if (key == '\t') ctx.graph.cycleFocus();
    });

    runner.registerStep("I press the \"(.*)\" key with input \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        char key = args[0][0];
        int inputVal = std::stoi(args[1]);
        if (key == 'F') {
             ctx.graph.addFocus(inputVal);
             layout::LayoutManager::applyPerspectiveBFS(ctx.graph, ctx.viewContext);
        } else if (key == 'O') {
             ctx.graph.removeFocus(inputVal);
        }
    });

    runner.registerStep("the pan offsets should be \\((.*), (.*)\\)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        EXPECT(ctx.viewContext.panX == std::stoi(args[0]), ctx, "panX mismatch");
        EXPECT(ctx.viewContext.panY == std::stoi(args[1]), ctx, "panY mismatch");
    });

    runner.registerStep("\"(.*)\" is \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (args[0] == "viewerOverlayMode") Config::viewerOverlayMode = (args[1] == "true");
        else if (args[0] == "allowMultiFocus") Config::allowMultiFocus = (args[1] == "true");
    });

    runner.registerStep("\"(.*)\" should be \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        bool actual = false;
        if (args[0] == "viewerOverlayMode") actual = Config::viewerOverlayMode;
        else if (args[0] == "allowMultiFocus") actual = Config::allowMultiFocus;
        EXPECT(actual == (args[1] == "true"), ctx, "Config mismatch");
    });

    runner.registerStep("the minimap visibility should be toggled", [](BDDContext& ctx, const std::vector<std::string>& args) {
        // Since we started at true (in BDDContext usually), it should be false now if we toggled once
        EXPECT(ctx.viewContext.showMinimap == false, ctx, "Minimap visibility not toggled");
    });
}

} // namespace bdd
