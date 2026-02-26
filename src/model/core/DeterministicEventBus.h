#pragma once

#include "model/core/contracts/IEventBus.h"
#include <map>
#include <mutex>
#include <vector>

namespace brain_model::core {

class DeterministicEventBus : public contracts::IEventBus {
public:
    void publish(const contracts::Event& event) override;
    void subscribe(const std::string& type, contracts::EventHandler handler) override;

    void start_recording() override;
    void stop_recording() override;
    void replay(const std::vector<contracts::Event>& history) override;
    std::vector<contracts::Event> get_recorded_history() const override;

private:
    std::map<std::string, std::vector<contracts::EventHandler>> m_subscribers;
    std::vector<contracts::Event> m_history;
    bool m_recording = false;
    mutable std::mutex m_mutex;
};

} // namespace brain_model::core
