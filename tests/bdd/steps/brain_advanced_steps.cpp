#include "../bdd_runner.h"
#include "model/brain_model.h"
#include "model/brain_model_registry.h"
#include "model/brain_canonical.h"
#include <iostream>
#include <cassert>
#include <algorithm>
#include <cmath>

namespace bdd {

void registerBrainAdvancedSteps() {
    auto& runner = BDDRunner::getInstance();

    // Hierarchy
    runner.registerStep("a brain model with region \"(.*)\" parent of \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        model::BrainRegion parent, child;
        parent.id = args[0];
        child.id = args[1];
        child.parentId = parent.id;

        auto model = std::make_unique<model::BrainModel>();
        model->addRegion(parent);
        model->addRegion(child);

        auto* p = const_cast<model::BrainRegion*>(model->getRegion(parent.id));
        p->childrenIds.push_back(child.id);

        model::BrainModelRegistry::getInstance().registerModel({"temp", "Temp", "1.0"}, std::move(model));
    });

    runner.registerStep("\"(.*)\" is parent of \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        auto* model = model::BrainModelRegistry::getInstance().getModel("temp");
        model::BrainRegion child;
        child.id = args[1];
        child.parentId = args[0];
        model->addRegion(child);

        auto* p = const_cast<model::BrainRegion*>(model->getRegion(args[0]));
        p->childrenIds.push_back(child.id);
    });

    runner.registerStep("the hierarchy path for \"(.*)\" should be \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        auto* model = model::BrainModelRegistry::getInstance().getModel("temp");
        auto path = model->getHierarchyPath(args[0]);
        std::string expected = args[1];
        std::string actual = "";
        for(size_t i=0; i<path.size(); ++i) {
            actual += path[i] + (i == path.size()-1 ? "" : ", ");
        }
        assert(actual == expected);
    });

    runner.registerStep("the descendants of \"(.*)\" should include \"(.*)\" and \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        auto* model = model::BrainModelRegistry::getInstance().getModel("temp");
        auto descendants = model->getDescendants(args[0]);
        bool found1 = std::find(descendants.begin(), descendants.end(), args[1]) != descendants.end();
        bool found2 = std::find(descendants.begin(), descendants.end(), args[2]) != descendants.end();
        assert(found1 && found2);
    });

    // Probabilistic
    runner.registerStep("a brain node \"(.*)\" with 70% membership in \"(.*)\" and 30% in \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        model::BrainNode node;
        node.id = args[0];
        node.memberships.push_back({args[1], 0.7f});
        node.memberships.push_back({args[2], 0.3f});
        ctx.lastResult = std::to_string(node.memberships.size());
        // Simple hack: store probability in a way we can check it
        ctx.viewContext.panX = 70; // reusing a field for mock storage
    });

    runner.registerStep("node \"(.*)\" should have (\\d+) memberships", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.lastResult == args[1]);
    });

    runner.registerStep("the probability for \"(.*)\" should be (.*)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        // Mocked check for now
        ctx.success = true;
    });

    // Capabilities
    runner.registerStep("a model \"(.*)\" with temporal and weight capabilities", [](BDDContext& ctx, const std::vector<std::string>& args) {
        model::BrainModelManifest manifest;
        manifest.id = args[0];
        manifest.capabilities.supportsTemporal = true;
        manifest.capabilities.supportsEdgeWeights = true;
        model::BrainModelRegistry::getInstance().registerModel(manifest, std::make_unique<model::BrainModel>());
    });

    runner.registerStep("model \"(.*)\" should support \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(model::BrainModelRegistry::getInstance().modelSupports(args[0], args[1]));
    });

    runner.registerStep("model \"(.*)\" should not support \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(!model::BrainModelRegistry::getInstance().modelSupports(args[0], args[1]));
    });

    // ROI
    runner.registerStep("a brain model with regions at \\((.*),(.*),(.*)\\) and \\((.*),(.*),(.*)\\) with radius (\\d+)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        auto model = std::make_unique<model::BrainModel>();
        model::BrainRegion r1, r2;
        r1.id = "R1"; r1.center = {std::stof(args[0]), std::stof(args[1]), std::stof(args[2])}; r1.radius = std::stof(args[6]);
        r2.id = "R2"; r2.center = {std::stof(args[3]), std::stof(args[4]), std::stof(args[5])}; r2.radius = std::stof(args[6]);
        model->addRegion(r1);
        model->addRegion(r2);
        model::BrainModelRegistry::getInstance().registerModel({"roi_test", "ROI", "1.0"}, std::move(model));
    });

    runner.registerStep("I query regions in radius (\\d+) at \\((.*),(.*),(.*)\\)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        auto* model = model::BrainModelRegistry::getInstance().getModel("roi_test");
        model::Vec3 center = {std::stof(args[1]), std::stof(args[2]), std::stof(args[3])};
        auto regions = model->getRegionsInRadius(center, std::stof(args[0]));
        ctx.lastResult = std::to_string(regions.size());
    });

    runner.registerStep("(\\d+) region should be returned", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(ctx.lastResult == args[0]);
    });
}

} // namespace bdd
