#ifndef PAGE_VIEW_H
#define PAGE_VIEW_H

#include "../map_logic.h"

namespace layout {

class PageView {
public:
    static void renderNodePage(const Graph& graph, int nodeId);
};

} // namespace layout

#endif // PAGE_VIEW_H
