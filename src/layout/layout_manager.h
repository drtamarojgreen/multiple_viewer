#ifndef LAYOUT_MANAGER_H
#define LAYOUT_MANAGER_H

#include "../map_logic.h"

namespace layout {

class LayoutManager {
public:
    static void applyForceDirected(Graph& graph, ViewContext& view);
    static void applyCircular(Graph& graph);
    static void applyRadial(Graph& graph);
    static void applyHierarchical(Graph& graph);
    static void applyPerspectiveBFS(Graph& graph, ViewContext& view);
};

} // namespace layout

#endif // LAYOUT_MANAGER_H
