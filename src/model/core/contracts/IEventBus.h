#pragma once

#include <functional>
#include <string>
#include <vector>
#include <memory>
#include <any>
#include <cstdint>

namespace brain_model::core::contracts {

struct Event {
    std::string type;
    uint64_t timestamp_ms;
    std::any payload;
};

using EventHandler = std::function<void(const Event&)>;

class IEventBus {
public:
    virtual ~IEventBus() = default;

    // Publish/Subscribe
    virtual void publish(const Event& event) = 0;
    virtual void subscribe(const std::string& type, EventHandler handler) = 0;

    // Deterministic Replay
    virtual void start_recording() = 0;
    virtual void stop_recording() = 0;
    virtual void replay(const std::vector<Event>& history) = 0;
    
    virtual std::vector<Event> get_recorded_history() const = 0;
};

} // namespace brain_model::core::contracts
