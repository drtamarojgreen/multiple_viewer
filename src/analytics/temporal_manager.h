#ifndef TEMPORAL_MANAGER_H
#define TEMPORAL_MANAGER_H

#include "../map_logic.h"
#include <vector>
#include <map>

namespace analytics {

struct GraphSnapshot {
    int timestamp;
    Graph graphState;
};

class TemporalManager {
public:
    void captureSnapshot(const Graph& graph, int timestamp);
    const Graph* getSnapshot(int timestamp) const;

private:
    std::map<int, GraphSnapshot> snapshots_;
};

} // namespace analytics

#endif // TEMPORAL_MANAGER_H
