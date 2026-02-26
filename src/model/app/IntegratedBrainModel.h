#include "../core/contracts/ISimulationKernel.h"
#include "../core/contracts/IOverlayService.h"
#include <vector>
#include <memory>

namespace brain_model::app {

using namespace brain_model::core::contracts;

class IntegratedBrainModel {
public:
    IntegratedBrainModel(
        std::shared_ptr<ISimulationKernel> kernel,
        std::shared_ptr<IOverlayService> overlays
    ) : m_kernel(kernel), m_overlays(overlays) {}

    void add_domain(std::shared_ptr<IModelDomainPlugin> domain) {
        m_domains.push_back(domain);
    }

    void run_step(uint32_t delta_ms) {
        m_kernel->step(delta_ms);
        
        // In a full implementation, we would iterate through registered systems
        // For this text-based integrated model demonstration, we trigger updates
        std::cout << "--- [Sim Step: " << m_kernel->current_time_ms() << "ms] ---" << std::endl;
        
        // Mock update for demonstration
        for (const auto& domain : m_domains) {
            std::cout << "Updating Domain: " << domain->name() << std::endl;
        }
    }

private:
    std::shared_ptr<ISimulationKernel> m_kernel;
    std::shared_ptr<IOverlayService> m_overlays;
    std::vector<std::shared_ptr<IModelDomainPlugin>> m_domains;
};

} // namespace brain_model::app
