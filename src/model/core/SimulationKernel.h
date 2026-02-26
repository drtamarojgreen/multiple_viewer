#pragma once

#include "model/core/contracts/ISimulationKernel.h"
<<<<<<< HEAD
=======
#include <chrono>
>>>>>>> main
#include <random>

namespace brain_model::core {

<<<<<<< HEAD
using namespace brain_model::core::contracts;

class SimulationKernel : public ISimulationKernel {
=======
class SimulationKernel : public contracts::ISimulationKernel {
>>>>>>> main
public:
    SimulationKernel();

    void step(uint32_t delta_ms) override;
    void pause() override;
    void resume() override;

<<<<<<< HEAD
    SimulationSnapshot capture_snapshot() const override;
    void restore_snapshot(const SimulationSnapshot& snapshot) override;

    void set_seed(uint64_t seed) override;
=======
    contracts::SimulationSnapshot capture_snapshot() const override;
    void restore_snapshot(const contracts::SimulationSnapshot& snapshot) override;

    void set_seed(uint64_t seed) override;

>>>>>>> main
    uint64_t current_time_ms() const override;
    bool is_running() const override;

private:
    uint64_t m_currentTimeMs = 0;
    uint64_t m_seed = 42;
<<<<<<< HEAD
    uint32_t m_timestepMs = 16; // 60fps fixed
=======
    uint32_t m_timestepMs = 16;
>>>>>>> main
    bool m_running = false;
    std::mt19937_64 m_rng;
};

} // namespace brain_model::core
