#include "layout_manager.h"
#include <iostream>

namespace layout {

void LayoutManager::applyForceDirected(Graph& graph) {
    std::cout << "[Layout] Applying Force-Directed layout...\n";
}

void LayoutManager::applyCircular(Graph& graph) {
    std::cout << "[Layout] Applying Circular layout...\n";
}

void LayoutManager::applyRadial(Graph& graph) {
    std::cout << "[Layout] Applying Radial layout...\n";
}

void LayoutManager::applyHierarchical(Graph& graph) {
    std::cout << "[Layout] Applying Hierarchical layout...\n";
}

} // namespace layout
