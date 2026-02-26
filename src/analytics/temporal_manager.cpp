#include "temporal_manager.h"

namespace analytics {

void TemporalManager::captureSnapshot(const Graph& graph, int timestamp) {
    GraphSnapshot snapshot;
    snapshot.timestamp = timestamp;
    snapshot.graphState = graph;
    snapshots_[timestamp] = snapshot;
}

const Graph* TemporalManager::getSnapshot(int timestamp) const {
    if (snapshots_.count(timestamp)) {
        return &snapshots_.at(timestamp).graphState;
    }
    return nullptr;
}

} // namespace analytics
