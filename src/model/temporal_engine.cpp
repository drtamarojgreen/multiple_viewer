#include "temporal_engine.h"
#include <algorithm>

namespace model {

TemporalEngine::TemporalEngine() : currentPositionMs_(0), isPlaying_(false) {}

void TemporalEngine::addFrame(const TemporalFrame& frame) {
    frames_.push_back(frame);
    std::sort(frames_.begin(), frames_.end(), [](const TemporalFrame& a, const TemporalFrame& b) {
        return a.timestamp_ms < b.timestamp_ms;
    });
}

void TemporalEngine::addBookmark(const TimelineBookmark& bookmark) {
    bookmarks_.push_back(bookmark);
}

void TemporalEngine::setTimelinePosition(uint64_t timestamp_ms) {
    currentPositionMs_ = timestamp_ms;
}

void TemporalEngine::play() { isPlaying_ = true; }
void TemporalEngine::pause() { isPlaying_ = false; }

void TemporalEngine::stepForward() {
    currentPositionMs_ += 100; // Mock step
}

void TemporalEngine::stepBackward() {
    if (currentPositionMs_ >= 100) currentPositionMs_ -= 100;
}

std::vector<float> TemporalEngine::getCurrentInterpolatedData() const {
    if (frames_.empty()) return {};
    if (frames_.size() == 1) return frames_[0].data;

    // Find bounding frames
    auto it = std::lower_bound(frames_.begin(), frames_.end(), currentPositionMs_,
        [](const TemporalFrame& f, uint64_t ts) {
            return f.timestamp_ms < ts;
        });

    if (it == frames_.begin()) return frames_.front().data;
    if (it == frames_.end()) return frames_.back().data;

    const auto& f2 = *it;
    const auto& f1 = *(--it);

    float t = static_cast<float>(currentPositionMs_ - f1.timestamp_ms) / (f2.timestamp_ms - f1.timestamp_ms);

    std::vector<float> result;
    size_t size = std::min(f1.data.size(), f2.data.size());
    result.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        result.push_back(f1.data[i] * (1.0f - t) + f2.data[i] * t);
    }
    return result;
}

} // namespace model
