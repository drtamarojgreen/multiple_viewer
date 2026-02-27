#ifndef BDD_RUNNER_H
#define BDD_RUNNER_H

#include <string>
#include <vector>
#include <functional>
#include <map>
#include <regex>
#include <utility>
#include <memory>
#include "../../src/input/command_stack.h"
#include "../../src/map_logic.h"
#include "../../src/model/brain_model.h"
#include "../../src/model/brain_overlay.h"
#include "../../src/analytics/analytics_engine_ext.h"
#include "../../src/analytics/temporal_manager.h"
#include "../../src/render/spatial_index.h"
#include "../../src/io/web_server_stub.h"
#include "../../src/model/core/SimulationKernel.h"
#include "../../src/model/app/IntegratedBrainModel.h"
#include "../../src/model/core/contracts/IOverlayService.h"
#include "../../src/model/core/contracts/ISimulationKernel.h"
#include "../../src/model/core/OverlayService.h"
#include "../../src/model/temporal_engine.h"
#include "../print/UIPrinter.h"

namespace bdd {

using namespace input;
using namespace analytics;
using namespace io;
using namespace brain_model::core;
using namespace brain_model::app;
using namespace brain_model::core::contracts;
using namespace render;

struct BDDContext {
    Graph graph;
    model::BrainModel brainModel;
    model::BrainOverlay overlay;
    CommandStack commandStack;
    ViewContext viewContext;
    bool minimapVisible = false;
    std::string minimapFocusArea;
    bool saveGraphCommandExecuted = false;
    bool svgExported = false;
    CentralityMetrics centralityMetrics;
    bool pluginLoaded = false;
    std::string loadedPluginName;
    TemporalManager temporalManager;
    std::string hypothesisAnnotation;
    int initialGraphNodeCount;
    int modifiedGraphNodeCount;
    OctreeIndex spatialIndex = OctreeIndex({-1000, -1000, -1000, 1000, 1000, 1000}, 8, 0);
    int queryResultCount;
    WebServerStub webServer;
    std::string webServerResponse;
    std::string currentMenu;
    std::string helpMessage;
    bool benchmarkSuiteReady = false;

    // Core Simulation Components
    std::unique_ptr<SimulationKernel> simulationKernel;
    std::unique_ptr<SimulationKernel> simulationKernel2;
    SimulationSnapshot snapshot1;
    SimulationSnapshot snapshot2;
    std::unique_ptr<IntegratedBrainModel> integratedBrainModel;
    std::shared_ptr<IOverlayService> mockOverlayService;
    std::shared_ptr<ISimulationKernel> mockSimulationKernel;

    // Concrete instances
    brain_model::core::SimulationKernel kernel;
    brain_model::core::OverlayService overlayService;
    model::TemporalEngine temporalEngine;

    std::unique_ptr<print::UIPrinter> uiPrinter;
    std::string lastResult;
    bool success = true;

    BDDContext() = default;
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
    
    std::vector<std::pair<std::regex, StepFunc>> steps_;
    
    bool executeLine(BDDContext& ctx, const std::string& line);
    std::string trim(const std::string& s);
};

// Macro for registration
#define GIVEN(pattern, ctx, args) BDDRunner::getInstance().registerStep(pattern, [](BDDContext& ctx, const std::vector<std::string>& args)
#define WHEN(pattern, ctx, args) BDDRunner::getInstance().registerStep(pattern, [](BDDContext& ctx, const std::vector<std::string>& args)
#define THEN(pattern, ctx, args) BDDRunner::getInstance().registerStep(pattern, [](BDDContext& ctx, const std::vector<std::string>& args)

} // namespace bdd

#endif // BDD_RUNNER_H
