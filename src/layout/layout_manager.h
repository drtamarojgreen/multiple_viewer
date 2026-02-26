#ifndef LAYOUT_MANAGER_H
#define LAYOUT_MANAGER_H

#include "../map_logic.h"

namespace layout {

class LayoutManager {
public:
    static void applyForceDirected(Graph& graph);
    static void applyCircular(Graph& graph);
    static void applyRadial(Graph& graph);
    static void applyHierarchical(Graph& graph);
};

} // namespace layout

#endif // LAYOUT_MANAGER_H
