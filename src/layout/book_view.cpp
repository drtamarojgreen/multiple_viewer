#include "book_view.h"
#include <iostream>
#include <map>
#include <algorithm>

namespace layout {

void BookView::render(Graph& graph, const ViewContext& view, const SearchState& search) {
    std::cout << "=== CBT Graph Viewer (Book View) ===\n";

    auto chapters = createBookStructure(graph, view);
    int row = 2; // Start rendering from the second row

    for (const auto& ch : chapters) {
        if (row >= view.height - 1) break;
        std::cout << "\n-- " << ch.chapterTitle << " (Depth " << ch.chapterDepth << ") --\n";
        row++;

        for (int nodeId : ch.nodeIds) {
            if (row >= view.height - 1) break;
            bool isFocused = graph.isNodeFocused(nodeId);
            bool isMatch = search.isActive && std::find(search.matches.begin(), search.matches.end(), nodeId) != search.matches.end();
            bool isActiveMatch = search.getActiveMatchNodeId() == nodeId;

            if (isActiveMatch) std::cout << " >>";
            else if (isMatch) std::cout << "  >";
            else if (isFocused) std::cout << "  o";
            else std::cout << "   ";

            std::cout << "[" << graph.nodeMap.at(nodeId).label << "]\n";
            row++;
        }
    }
}

std::vector<BookChapter> BookView::createBookStructure(const Graph& g, const ViewContext& view) {
    std::map<std::string, BookChapter> chMap;
    for (const auto& node : g.nodes) {
        int depth = (g.nodePos.count(node.index)
                   ? static_cast<int>(g.nodePos.at(node.index).z) : 0);

        std::string key = std::to_string(node.subjectIndex) + "_" + std::to_string(depth);
        auto& ch = chMap[key];
        ch.chapterTitle = "Subject " + std::to_string(node.subjectIndex);
        ch.chapterDepth = depth;
        ch.nodeIds.push_back(node.index);
    }
    std::vector<BookChapter> res;
    for (auto &kv : chMap) res.push_back(kv.second);
    return res;
}

} // namespace layout
