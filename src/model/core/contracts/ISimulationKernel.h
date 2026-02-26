#pragma once

#include <cstdint>
#include <vector>
#include <optional>
#include <string>

namespace brain_model::core::contracts {

struct SimulationSnapshot {
    uint64_t timestamp_ms;
    std::vector<uint8_t> state_blob;
    std::string snapshot_hash;
};

class ISimulationKernel {
public:
    virtual ~ISimulationKernel() = default;

    // Execution Control
    virtual void step(uint32_t delta_ms) = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;

    // Snapshot & Determinism
    virtual SimulationSnapshot capture_snapshot() const = 0;
    virtual void restore_snapshot(const SimulationSnapshot& snapshot) = 0;
    
    // Seeded RNG input for deterministic behavior
    virtual void set_seed(uint64_t seed) = 0;
    
    virtual uint64_t current_time_ms() const = 0;
    virtual bool is_running() const = 0;
};

} // namespace brain_model::core::contracts
