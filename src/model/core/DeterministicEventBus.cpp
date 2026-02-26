<<<<<<< HEAD
#include "model/core/contracts/IEventBus.h"
#include <map>
#include <mutex>
=======
#include "model/core/DeterministicEventBus.h"
>>>>>>> main
#include <algorithm>

namespace brain_model::core {

void DeterministicEventBus::publish(const contracts::Event& event) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_recording) {
        m_history.push_back(event);
    }

    if (m_subscribers.count(event.type)) {
        for (const auto& handler : m_subscribers[event.type]) {
            handler(event);
        }
    }
}

void DeterministicEventBus::subscribe(const std::string& type, contracts::EventHandler handler) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_subscribers[type].push_back(handler);
}

void DeterministicEventBus::start_recording() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_recording = true;
    m_history.clear();
}

void DeterministicEventBus::stop_recording() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_recording = false;
}

void DeterministicEventBus::replay(const std::vector<contracts::Event>& history) {
    for (const auto& event : history) {
        publish(event);
    }
}

std::vector<contracts::Event> DeterministicEventBus::get_recorded_history() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_history;
}

} // namespace brain_model::core
