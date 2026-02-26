<<<<<<< HEAD
#include "SimulationKernel.h"
#include <chrono>
#include <algorithm>
=======
#include "model/core/SimulationKernel.h"
>>>>>>> main
#include <cstring>

namespace brain_model::core {

SimulationKernel::SimulationKernel() : m_rng(m_seed) {}

void SimulationKernel::step(uint32_t delta_ms) {
    if (!m_running) return;
    m_currentTimeMs += delta_ms;
<<<<<<< HEAD
    // In a real implementation, this would trigger system updates
=======
>>>>>>> main
}

void SimulationKernel::pause() { m_running = false; }
void SimulationKernel::resume() { m_running = true; }

<<<<<<< HEAD
SimulationSnapshot SimulationKernel::capture_snapshot() const {
    SimulationSnapshot snapshot;
    snapshot.timestamp_ms = m_currentTimeMs;
    // Simple state blob for now
=======
contracts::SimulationSnapshot SimulationKernel::capture_snapshot() const {
    contracts::SimulationSnapshot snapshot;
    snapshot.timestamp_ms = m_currentTimeMs;
>>>>>>> main
    snapshot.state_blob.resize(sizeof(m_currentTimeMs) + sizeof(m_seed));
    std::memcpy(snapshot.state_blob.data(), &m_currentTimeMs, sizeof(m_currentTimeMs));
    std::memcpy(snapshot.state_blob.data() + sizeof(m_currentTimeMs), &m_seed, sizeof(m_seed));
    snapshot.snapshot_hash = "hash_" + std::to_string(m_currentTimeMs);
    return snapshot;
}

<<<<<<< HEAD
void SimulationKernel::restore_snapshot(const SimulationSnapshot& snapshot) {
=======
void SimulationKernel::restore_snapshot(const contracts::SimulationSnapshot& snapshot) {
>>>>>>> main
    if (snapshot.state_blob.size() >= sizeof(m_currentTimeMs) + sizeof(m_seed)) {
        std::memcpy(&m_currentTimeMs, snapshot.state_blob.data(), sizeof(m_currentTimeMs));
        std::memcpy(&m_seed, snapshot.state_blob.data() + sizeof(m_currentTimeMs), sizeof(m_seed));
        m_rng.seed(m_seed);
    }
}

void SimulationKernel::set_seed(uint64_t seed) {
    m_seed = seed;
    m_rng.seed(m_seed);
}

uint64_t SimulationKernel::current_time_ms() const { return m_currentTimeMs; }
bool SimulationKernel::is_running() const { return m_running; }

} // namespace brain_model::core
