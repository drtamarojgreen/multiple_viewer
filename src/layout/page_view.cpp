#include "page_view.h"
#include <iostream>

namespace layout {

void PageView::renderNodePage(const Graph& graph, int nodeId) {
    if (!graph.nodeExists(nodeId)) {
        std::cout << "[Page] Node " << nodeId << " does not exist.\n";
        return;
    }

    const auto& n = graph.nodeMap.at(nodeId);
    std::cout << "\n=== CBT Node Page View ===\n";
    std::cout << "Label: " << n.label << "\n";
    std::cout << "Index: " << n.index << "\n";
    std::cout << "Subject: " << n.subjectIndex << "\n";
    std::cout << "Weight: " << n.weight << "\n";
    if (!n.regionIds.empty()) {
        std::cout << "Brain Regions: ";
        for (size_t i = 0; i < n.regionIds.size(); ++i) {
            std::cout << n.regionIds[i] << (i == n.regionIds.size() - 1 ? "" : ", ");
        }
        std::cout << "\n";
    }

    if (!n.pathwayId.empty()) std::cout << "Brain Pathway: " << n.pathwayId << "\n";
    std::cout << "Parent: " << n.subjectIndex << "\n";
    std::cout << "Neighbors:\n";
    for (int nbr : n.neighbors)
        std::cout << "  • " << graph.nodeMap.at(nbr).label << "\n";

    graph.pause();
}

} // namespace layout
