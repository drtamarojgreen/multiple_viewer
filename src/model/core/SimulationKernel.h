#pragma once

#include "model/core/contracts/ISimulationKernel.h"
#include <random>

namespace brain_model::core {

using namespace brain_model::core::contracts;

class SimulationKernel : public ISimulationKernel {
public:
    SimulationKernel();

    void step(uint32_t delta_ms) override;
    void pause() override;
    void resume() override;

    SimulationSnapshot capture_snapshot() const override;
    void restore_snapshot(const SimulationSnapshot& snapshot) override;

    void set_seed(uint64_t seed) override;
    uint64_t current_time_ms() const override;
    bool is_running() const override;

private:
    uint64_t m_currentTimeMs = 0;
    uint64_t m_seed = 42;
    uint32_t m_timestepMs = 16; // 60fps fixed
    bool m_running = false;
    std::mt19937_64 m_rng;
};

} // namespace brain_model::core
