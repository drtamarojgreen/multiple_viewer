#ifndef TEMPORAL_ENGINE_H
#define TEMPORAL_ENGINE_H

#include <cstdint>
#include <vector>
#include <string>

namespace model {

struct TemporalFrame {
    uint64_t timestamp_ms;
    std::string state_id;
    std::vector<float> data;
};

struct TimelineBookmark {
    uint64_t timestamp_ms;
    std::string label;
    std::string category;
};

class TemporalEngine {
public:
    TemporalEngine();
    ~TemporalEngine() = default;

    void addFrame(const TemporalFrame& frame);
    void addBookmark(const TimelineBookmark& bookmark);
    std::vector<TimelineBookmark> getBookmarks() const { return bookmarks_; }
    void setTimelinePosition(uint64_t timestamp_ms);
    uint64_t getTimelinePosition() const { return currentPositionMs_; }

    void play();
    void pause();
    void stepForward();
    void stepBackward();

    bool isPlaying() const { return isPlaying_; }

    // Interpolation
    std::vector<float> getCurrentInterpolatedData() const;

private:
    std::vector<TemporalFrame> frames_;
    std::vector<TimelineBookmark> bookmarks_;
    uint64_t currentPositionMs_ = 0;
    bool isPlaying_ = false;
};

} // namespace model

#endif // TEMPORAL_ENGINE_H
