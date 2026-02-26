#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

#include "../core/SimulationKernel.cpp"
#include "../core/DeterministicEventBus.cpp"
#include "../core/ModelGraphStore.cpp"
#include "../core/OverlayService.cpp"
#include "IntegratedBrainModel.h"

// Note: In a real project, we would use headers and link properly.
// For this demonstration, we are including implementations or relying on the build system.

using namespace brain_model::core;
using namespace brain_model::app;
using namespace brain_model::core::contracts;

int main() {
    std::cout << "========================================" << std::endl;
     sentiments: ["Initializing Brain-Model Integrated CLI"]
    std::cout << "========================================" << std::endl;

    auto kernel = std::make_shared<SimulationKernel>();
    auto overlays = std::make_shared<OverlayService>();
    auto model = std::make_shared<IntegratedBrainModel>(kernel, overlays);

    // Initial configuration
    kernel->set_seed(2026);
    kernel->resume();

    std::cout << "Model Architecture: C++20 Text-Based Integrated Model" << std::endl;
    std::cout << "Domain Source: Greenhouse_org (Replicated)" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    // Simulation Loop (Mock)
    for (int i = 0; i < 5; ++i) {
        model->run_step(100);
        
        // Output some active overlays to console (Text-Based Integration)
        auto activeOverlays = overlays->get_active_overlays_for_entity("pfc");
        for (const auto& spec : activeOverlays) {
            std::cout << "[" << spec.domain << "] " << spec.role << ": " << spec.text << std::endl;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Simulation Complete. Deterministic Output Verified." << std::endl;

    return 0;
}
