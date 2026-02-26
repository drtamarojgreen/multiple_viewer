#ifndef BDD_RUNNER_H
#define BDD_RUNNER_H

#include <string>
#include <vector>
#include <functional>
#include <map>
#include "../../src/input/command_stack.h"
#include "../../src/map_logic.h"
#include "../../src/model/brain_model.h"
#include "../../src/model/brain_overlay.h"
#include "../../src/analytics/analytics_engine_ext.h"
#include "../../src/analytics/temporal_manager.h"
#include "../../src/render/spatial_index.h"
#include "../../src/io/web_server_stub.h"
#include "../../src/model/core/SimulationKernel.h" // Include SimulationKernel
#include "../../src/model/app/IntegratedBrainModel.h" // Include IntegratedBrainModel
#include "../../src/model/core/contracts/IOverlayService.h" // Include IOverlayService
#include "../../src/model/core/contracts/ISimulationKernel.h" // Include ISimulationKernel

namespace bdd {

struct BDDContext {
    Graph graph;
    model::BrainModel brainModel;
    model::BrainOverlay overlay;
    input::CommandStack commandStack;
    ViewContext viewContext; // Add ViewContext
    bool minimapVisible = false;
    std::string minimapFocusArea;
    bool saveGraphCommandExecuted = false;
    bool svgExported = false; // Add svgExported
    analytics::CentralityMetrics centralityMetrics; // Add centralityMetrics
    bool pluginLoaded = false;
    std::string loadedPluginName;
    analytics::TemporalManager temporalManager;
    std::string hypothesisAnnotation;
    int initialGraphNodeCount;
    int modifiedGraphNodeCount;
    render::OctreeIndex spatialIndex = render::OctreeIndex({-1000, -1000, -1000, 1000, 1000, 1000}, 8, 0);
    int queryResultCount;
    io::WebServerStub webServer;
    std::string webServerResponse;
    std::string currentMenu;
    std::string helpMessage;
    bool benchmarkSuiteReady = false;
    std::unique_ptr<brain_model::core::SimulationKernel> simulationKernel;
    std::unique_ptr<brain_model::core::SimulationKernel> simulationKernel2;
    brain_model::core::SimulationSnapshot snapshot1;
    brain_model::core::SimulationSnapshot snapshot2;
    std::unique_ptr<brain_model::app::IntegratedBrainModel> integratedBrainModel;
    std::shared_ptr<brain_model::core::contracts::IOverlayService> mockOverlayService;
    std::shared_ptr<brain_model::core::contracts::ISimulationKernel> mockSimulationKernel;
    std::string lastResult;
    bool success = true;
};

using StepFunc = std::function<void(BDDContext&, const std::vector<std::string>&)>;

class BDDRunner {
public:
    static BDDRunner& getInstance() {
        static BDDRunner instance;
        return instance;
    }

    void registerStep(const std::string& pattern, StepFunc func);
    bool runFeature(const std::string& filepath);

private:
    BDDRunner() = default;
    
    std::map<std::string, StepFunc> steps_;
    
    bool executeLine(BDDContext& ctx, const std::string& line);
    std::string trim(const std::string& s);
};

// Macro for registration
#define GIVEN(pattern, ctx, args) BDDRunner::getInstance().registerStep(pattern, [](BDDContext& ctx, const std::vector<std::string>& args)
#define WHEN(pattern, ctx, args) BDDRunner::getInstance().registerStep(pattern, [](BDDContext& ctx, const std::vector<std::string>& args)
#define THEN(pattern, ctx, args) BDDRunner::getInstance().registerStep(pattern, [](BDDContext& ctx, const std::vector<std::string>& args)

} // namespace bdd

#endif // BDD_RUNNER_H
