#include "../bdd_runner.h"
#include <iostream>
#include <cassert>

namespace bdd {

void registerBrainSteps() {
    auto& runner = BDDRunner::getInstance();

    runner.registerStep("a graph with a node \"(.*)\" at index (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::string label = args[0];
        int index = std::stoi(args[1]);
        ctx.graph.addNode(GraphNode(label, index));
        std::cout << "[STEP] Added node " << label << " at index " << index << "\n";
    });

    runner.registerStep("a brain model with region \"(.*)\" named \"(.*)\" at \\((.*), (.*), (.*)\\) with radius (.*)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        model::BrainRegion region;
        region.id = args[0];
        region.name = args[1];
        region.center = { std::stof(args[2]), std::stof(args[3]), std::stof(args[4]) };
        region.radius = std::stof(args[5]);
        ctx.brainModel.addRegion(region);
        std::cout << "[STEP] Added brain region " << region.id << " (" << region.name << ")\n";
    });

    runner.registerStep("I apply a brain overlay mapping node (\\d+) to region \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        int nodeId = std::stoi(args[0]);
        std::string regionId = args[1];
        model::OverlayMapping mapping;
        mapping.graphNodeId = nodeId;
        mapping.regionId = regionId;
        ctx.overlay.addMapping(mapping);
        // Apply to graph
        ctx.graph.applyBrainOverlay(ctx.overlay);
        std::cout << "[STEP] Mapped node " << nodeId << " to region " << regionId << "\n";
    });

    runner.registerStep("node (\\d+) should be associated with region \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        int nodeId = std::stoi(args[0]);
        std::string expectedRegion = args[1];
        std::string actualRegion = ctx.graph.nodeMap.at(nodeId).regionId;
        if (actualRegion != expectedRegion) {
            throw std::runtime_error("Expected region " + expectedRegion + " but found " + actualRegion);
        }
        std::cout << "[STEP] Verified node " << nodeId << " association with " << expectedRegion << "\n";
    });
}

} // namespace bdd
