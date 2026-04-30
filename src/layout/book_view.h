#ifndef BOOK_VIEW_H
#define BOOK_VIEW_H

#include "../map_logic.h"
#include "../search_logic.h"
#include <vector>
#include <string>

namespace layout {

struct BookChapter {
    std::string chapterTitle;
    int chapterDepth;
    std::vector<int> nodeIds;
};

class BookView {
public:
    static void render(Graph& graph, const ViewContext& view, const SearchState& search);
    static std::vector<BookChapter> createBookStructure(const Graph& g, const ViewContext& view);
};

} // namespace layout

#endif // BOOK_VIEW_H
