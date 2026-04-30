#ifndef NEXUS_FLOW_VIEW_H
#define NEXUS_FLOW_VIEW_H

#include "../map_logic.h"
#include "../search_logic.h"

namespace layout {

class NexusFlowView {
public:
    static void render(Graph& graph, NexusPhysicsState& physics, const SearchState& search);
};

} // namespace layout

#endif // NEXUS_FLOW_VIEW_H
