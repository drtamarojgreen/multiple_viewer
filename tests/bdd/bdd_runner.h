#ifndef BDD_RUNNER_H
#define BDD_RUNNER_H

#include <string>
#include <vector>
#include <functional>
#include <map>
#include <regex>
#include <utility>
#include <memory>
#include <iostream>
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

// Standard assertion macro for BDD steps
#ifndef EXPECT
#define EXPECT(condition, ctx, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "      [BDD ERROR] Assertion failed: " << message << "\n"; \
            (ctx).success = false; \
            return; \
        } \
    } while (0)
#endif

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
    int initialGraphNodeCount = 0;
    int modifiedGraphNodeCount = 0;
    std::unique_ptr<OctreeIndex> spatialIndex;
    int queryResultCount = 0;
    std::unique_ptr<WebServerStub> webServer;
    std::string webServerResponse;
    std::string currentMenu;
    std::string helpMessage;
    bool benchmarkSuiteReady = false;

    // Simulation components
    std::unique_ptr<SimulationKernel> simulationKernel;
    std::unique_ptr<SimulationKernel> simulationKernel2;
    SimulationSnapshot snapshot1;
    SimulationSnapshot snapshot2;
    std::unique_ptr<IntegratedBrainModel> integratedBrainModel;
    std::shared_ptr<IOverlayService> mockOverlayService;
    std::shared_ptr<ISimulationKernel> mockSimulationKernel;

    std::shared_ptr<ISimulationKernel> kernel;
    std::shared_ptr<IOverlayService> overlayService;

    model::TemporalEngine temporalEngine;

    std::unique_ptr<print::UIPrinter> uiPrinter;
    std::string lastResult;
    bool success = true;

    BDDContext() {
        spatialIndex = std::make_unique<OctreeIndex>(SpatialBounds{-1000, -1000, -1000, 1000, 1000, 1000}, 8, 0);
        webServer = std::make_unique<WebServerStub>();
        kernel = std::make_shared<SimulationKernel>();
        overlayService = std::make_shared<OverlayService>();
    }
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

} // namespace bdd

#endif // BDD_RUNNER_H
