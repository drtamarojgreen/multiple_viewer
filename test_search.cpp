#include <iostream>
#include <vector>
#include <string>
#include "src/search_logic.h"
#include "src/map_logic.h"

int main() {
    Graph g;
    GraphNode node("The quick brown fox", 0);
    g.addNode(node);
    std::cout << "Node count: " << g.nodes.size() << std::endl;
    if (g.nodes.size() > 0) std::cout << "First node label: " << g.nodes[0].label << std::endl;

    std::vector<int> results = findSimilarTopics(g, "fox");
    std::cout << "Results size: " << results.size() << std::endl;
    return 0;
}
