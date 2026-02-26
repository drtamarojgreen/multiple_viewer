#include "model/core/contracts/IEventBus.h"
#include <map>
#include <mutex>
#include <algorithm>

namespace brain_model::core {

using namespace brain_model::core::contracts;

class DeterministicEventBus : public IEventBus {
public:
    void publish(const Event& event) override {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        if (m_recording) {
            m_history.push_back(event);
        }

        auto it = m_handlers.find(event.type);
        if (it != m_handlers.end()) {
            for (auto& handler : it->second) {
                handler(event);
            }
        }
    }

    void subscribe(const std::string& type, EventHandler handler) override {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_handlers[type].push_back(handler);
    }

    void start_recording() override {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_recording = true;
        m_history.clear();
    }

    void stop_recording() override {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_recording = false;
    }

    void replay(const std::vector<Event>& history) override {
        for (const auto& event : history) {
            publish(event);
        }
    }

    std::vector<Event> get_recorded_history() const override {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_history;
    }

private:
    mutable std::mutex m_mutex;
    std::map<std::string, std::vector<EventHandler>> m_handlers;
    std::vector<Event> m_history;
    bool m_recording = false;
};

} // namespace brain_model::core
