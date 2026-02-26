#include "core/contracts/ISimulationKernel.h"
#include <chrono>
#include <random>
#include <algorithm>

namespace brain_model::core {

using namespace brain_model::core::contracts;

class SimulationKernel : public ISimulationKernel {
public:
    SimulationKernel() : m_rng(m_seed) {}

    void step(uint32_t delta_ms) override {
        if (!m_running) return;
        m_currentTimeMs += delta_ms;
        // In a real implementation, this would trigger system updates
    }

    void pause() override { m_running = false; }
    void resume() override { m_running = true; }

    SimulationSnapshot capture_snapshot() const override {
        SimulationSnapshot snapshot;
        snapshot.timestamp_ms = m_currentTimeMs;
        // Simple state blob for now
        snapshot.state_blob.resize(sizeof(m_currentTimeMs) + sizeof(m_seed));
        std::memcpy(snapshot.state_blob.data(), &m_currentTimeMs, sizeof(m_currentTimeMs));
        std::memcpy(snapshot.state_blob.data() + sizeof(m_currentTimeMs), &m_seed, sizeof(m_seed));
        snapshot.snapshot_hash = "hash_" + std::to_string(m_currentTimeMs);
        return snapshot;
    }

    void restore_snapshot(const SimulationSnapshot& snapshot) override {
        if (snapshot.state_blob.size() >= sizeof(m_currentTimeMs) + sizeof(m_seed)) {
            std::memcpy(&m_currentTimeMs, snapshot.state_blob.data(), sizeof(m_currentTimeMs));
            std::memcpy(&m_seed, snapshot.state_blob.data() + sizeof(m_currentTimeMs), sizeof(m_seed));
            m_rng.seed(m_seed);
        }
    }

    void set_seed(uint64_t seed) override {
        m_seed = seed;
        m_rng.seed(m_seed);
    }

    uint64_t current_time_ms() const override { return m_currentTimeMs; }
    bool is_running() const override { return m_running; }

private:
    uint64_t m_currentTimeMs = 0;
    uint64_t m_seed = 42;
    uint32_t m_timestepMs = 16; // 60fps fixed
    bool m_running = false;
    std::mt19937_64 m_rng;
};

} // namespace brain_model::core
