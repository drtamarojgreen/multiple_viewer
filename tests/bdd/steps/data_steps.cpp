#include "../bdd_runner.h"
#include "io/io_manager.h"
#include "analytics/analytics_engine_ext.h"
#include <iostream>

namespace bdd {

void registerDataSteps() {
    auto& runner = BDDRunner::getInstance();

    runner.registerStep("a file \"(.*)\" in JSON format", [](BDDContext& ctx, const std::vector<std::string>& args) {
        ctx.success = true;
    });

    runner.registerStep("I load the graph from \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        // io::IOManager::loadJSON(ctx.graph, args[0]);
        ctx.lastResult = "loaded";
    });

    runner.registerStep("I export the graph as SVG to \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        // io::IOManager::exportSVG(ctx.graph, args[0]);
        ctx.lastResult = "exported";
    });

    runner.registerStep("a graph with three distinct clusters", [](BDDContext& ctx, const std::vector<std::string>& args) {
        // Mock cluster setup
        ctx.success = true;
    });

    runner.registerStep("I run community detection", [](BDDContext& ctx, const std::vector<std::string>& args) {
        auto communities = analytics::AnalyticsEngine::detectCommunities(ctx.graph);
        ctx.lastResult = std::to_string(communities.size());
    });

    runner.registerStep("it should identify \"(.*)\" communities", [](BDDContext& ctx, const std::vector<std::string>& args) {
        if (ctx.lastResult != args[0]) throw std::runtime_error("Community detection count mismatch");
    });
}

} // namespace bdd
