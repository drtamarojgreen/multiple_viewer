#include "../bdd_runner.h"
#include "model/brain_model_registry.h"
#include "model/temporal_engine.h"
#include "render/render_layer_manager.h"
#include <iostream>
#include <cassert>
#include <cmath>

namespace bdd {

void registerBrainAPISteps() {
    auto& runner = BDDRunner::getInstance();

    // Registry
    runner.registerStep("a structural brain model manifest for \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        model::BrainModelManifest manifest;
        manifest.id = args[0];
        manifest.family = model::ModelFamily::STRUCTURAL;
        ctx.lastResult = manifest.id;
    });

    runner.registerStep("a structural brain model with (\\d+) regions", [](BDDContext& ctx, const std::vector<std::string>& args) {
        auto model = std::make_unique<model::BrainModel>();
        int count = std::stoi(args[0]);
        for (int i = 0; i < count; ++i) {
            model::BrainRegion r;
            r.id = "reg_" + std::to_string(i);
            model->addRegion(r);
        }
        std::cout << "[STEP] Created model with " << count << " regions\n";
    });

    runner.registerStep("I register the model in the BrainModelRegistry", [](BDDContext& ctx, const std::vector<std::string>& args) {
        model::BrainModelManifest manifest;
        manifest.id = "mni_v1";
        manifest.family = model::ModelFamily::STRUCTURAL;
        auto model = std::make_unique<model::BrainModel>();
        model::BrainModelRegistry::getInstance().registerModel(manifest, std::move(model));
    });

    runner.registerStep("I should be able to retrieve the model \"(.*)\" from the registry", [](BDDContext& ctx, const std::vector<std::string>& args) {
        auto* model = model::BrainModelRegistry::getInstance().getModel(args[0]);
        assert(model != nullptr);
    });

    runner.registerStep("the model family should be \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        auto* manifest = model::BrainModelRegistry::getInstance().getManifest("mni_v1");
        assert(manifest != nullptr);
        if (args[0] == "STRUCTURAL") assert(manifest->family == model::ModelFamily::STRUCTURAL);
    });

    // Temporal
    runner.registerStep("a temporal engine with frames at (\\d+)ms and (\\d+)ms", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.temporalEngine.setTimelinePosition(0);
    });

    runner.registerStep("the data at (\\d+)ms is \\((.*), (.*), (.*)\\)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        model::TemporalFrame frame;
        frame.timestamp_ms = std::stoull(args[0]);
        frame.data = { std::stof(args[1]), std::stof(args[2]), std::stof(args[3]) };
        ctx.temporalEngine.addFrame(frame);
    });

    runner.registerStep("I set the timeline position to (\\d+)ms", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.temporalEngine.setTimelinePosition(std::stoull(args[0]));
    });

    runner.registerStep("the interpolated data should be approximately \\((.*), (.*), (.*)\\)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        auto data = ctx.temporalEngine.getCurrentInterpolatedData();
        assert(data.size() == 3);
        assert(std::abs(data[0] - std::stof(args[0])) < 0.1f);
        assert(std::abs(data[1] - std::stof(args[1])) < 0.1f);
        assert(std::abs(data[2] - std::stof(args[2])) < 0.1f);
    });

    // Layers
    runner.registerStep("a render layer \"(.*)\" of type \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        render::RenderLayer layer;
        layer.id = args[0];
        if (args[1] == "SURFACE") layer.type = render::LayerType::SURFACE;
        render::RenderLayerManager::getInstance().addLayer(layer);
    });

    runner.registerStep("I add the layer to the RenderLayerManager", [](BDDContext& ctx, const std::vector<std::string>& args) {
        // Already handled in Given
    });

    runner.registerStep("I set the visibility of \"(.*)\" to (.*)", [](BDDContext& ctx, const std::vector<std::string>& args) {
        bool visible = (args[1] == "true" || args[1] == "false" ? (args[1] == "true") : false);
        render::RenderLayerManager::getInstance().setVisibility(args[0], visible);
    });

    runner.registerStep("the layer \"(.*)\" should not be visible", [](BDDContext& ctx, const std::vector<std::string>& args) {
        assert(!render::RenderLayerManager::getInstance().isVisible(args[0]));
    });
}

} // namespace bdd
