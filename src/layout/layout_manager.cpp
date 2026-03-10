#include "layout_manager.h"
#include <iostream>
#include <cmath>
#include <queue>
#include <tuple>
#include <map>

namespace layout {

void LayoutManager::applyForceDirected(Graph& graph, ViewContext& view) {
    // Moved from viewer_logic.cpp renderNexusFlow
    const float k_repel = 2000.0f;
    const float k_attract = 0.05f;
    const float ideal_dist = 15.0f;
    const float damping = 0.85f;
    const int iterations = 5;

    static std::map<int, Point2D> velocities;

    if (graph.needsLayoutReset) {
        graph.layoutPositions.clear();
        velocities.clear();
        for (const auto& node : graph.nodes) {
            graph.layoutPositions[node.index] = { static_cast<float>(rand() % view.width),
                                                  static_cast<float>(rand() % view.height) };
            velocities[node.index] = { 0.0f, 0.0f };
        }
        graph.needsLayoutReset = false;
    }

    for (int i = 0; i < iterations; ++i) {
        std::map<int, Point2D> forces;
        for (const auto& n1 : graph.nodes) {
            for (const auto& n2 : graph.nodes) {
                if (n1.index == n2.index) continue;
                float dx = graph.layoutPositions[n1.index].x - graph.layoutPositions[n2.index].x;
                float dy = graph.layoutPositions[n1.index].y - graph.layoutPositions[n2.index].y;
                float dist_sq = dx * dx + dy * dy;
                if (dist_sq < 1.0f) dist_sq = 1.0f;
                float force = k_repel / dist_sq;
                forces[n1.index].x += dx * force;
                forces[n1.index].y += dy * force;
            }
        }
        for (const auto& node : graph.nodes) {
            for (int neighbor_id : node.neighbors) {
                if (graph.layoutPositions.count(neighbor_id) == 0) continue;
                float dx = graph.layoutPositions[neighbor_id].x - graph.layoutPositions[node.index].x;
                float dy = graph.layoutPositions[neighbor_id].y - graph.layoutPositions[node.index].y;
                float dist = std::sqrt(dx * dx + dy * dy);
                if (dist < 1.0f) dist = 1.0f;
                float force = k_attract * std::log(dist / ideal_dist);
                forces[node.index].x += dx * force;
                forces[node.index].y += dy * force;
                forces[neighbor_id].x -= dx * force;
                forces[neighbor_id].y -= dy * force;
            }
        }
        for (auto& node : graph.nodes) {
            velocities[node.index].x = (velocities[node.index].x + forces[node.index].x) * damping;
            velocities[node.index].y = (velocities[node.index].y + forces[node.index].y) * damping;
            graph.layoutPositions[node.index].x += velocities[node.index].x;
            graph.layoutPositions[node.index].y += velocities[node.index].y;
            graph.layoutPositions[node.index].x = std::max(0.0f, std::min(graph.layoutPositions[node.index].x, static_cast<float>(view.width - 1)));
            graph.layoutPositions[node.index].y = std::max(0.0f, std::min(graph.layoutPositions[node.index].y, static_cast<float>(view.height - 1)));
        }
    }
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

void LayoutManager::applyPerspectiveBFS(Graph& graph, ViewContext& view) {
    // Moved from ConsoleRenderer::render / viewer_logic.cpp renderGraph
    graph.layoutPositions.clear();
    graph.nodePos.clear();
    std::queue<std::tuple<int, int, int>> q;

    if (graph.focusedNodeIndices.empty() || graph.focusedNodeIndices.size() == 1) {
        int focus = -1;
        if (!graph.focusedNodeIndices.empty()) {
            focus = *graph.focusedNodeIndices.begin();
        } else if (!graph.nodes.empty()) {
            focus = graph.nodes.front().index;
        }

        if (graph.nodeExists(focus)) {
            float baseRow = (view.height - 1) / 2.0f;
            float baseCol = (view.width - 1) / 2.0f;
            graph.layoutPositions[focus] = { baseCol, baseRow };
            graph.nodePos[focus] = { baseRow, baseCol, 0.0f };
            q.push({ focus, static_cast<int>(baseRow), static_cast<int>(baseCol) });
        }
    } else {
        const auto& focused_nodes = graph.focusedNodeIndices;
        int num_focused = focused_nodes.size();
        int spacing = view.width / (num_focused + 1);
        int i = 1;
        for (int focus_id : focused_nodes) {
            if (graph.nodeExists(focus_id)) {
                float row = view.height / 2.0f;
                float col = static_cast<float>(i * spacing);
                graph.layoutPositions[focus_id] = { col, row };
                graph.nodePos[focus_id] = { row, col, 0.0f };
                q.push({ focus_id, static_cast<int>(row), static_cast<int>(col) });
                i++;
            }
        }
    }

    if (q.empty()) return;

    int maxDist = view.maxRenderDistance;

    while (!q.empty()) {
        auto [u, wr, wc] = q.front(); q.pop();
        int dz = graph.nodePos[u].z;
        if (dz >= maxDist) continue;

        const auto& u_node = graph.nodeMap.at(u);
        const std::vector<std::pair<int, int>> directions = {
            {0, 1}, {1, 0}, {0, -1}, {-1, 0},
            {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
        };
        int dir_idx = 0;

        for (int v : u_node.neighbors) {
            if (graph.nodePos.count(v)) continue;
            if (dir_idx >= (int)directions.size()) break;

            auto [dr, dc] = directions[dir_idx++];
            int nz = dz + 1;
            if (nz > maxDist) continue;

            int parent_size = graph.calculateNodeSize(dz, view.zoomLevel);
            int child_size = graph.calculateNodeSize(nz, view.zoomLevel);
            int step = (parent_size / 2) + (child_size / 2) + Config::nodePadding + 2;
            int nr = wr + dr * step;
            int nc = wc + dc * step;

            bool collision = false;
            for (const auto& [other_id, other_pos] : graph.nodePos) {
                int other_node_size = graph.calculateNodeSize(other_pos.z, view.zoomLevel);
                if (std::abs(nr - other_pos.x) * 2 < (child_size + other_node_size) &&
                    std::abs(nc - other_pos.y) * 2 < (child_size + other_node_size)) {
                    collision = true;
                    break;
                }
            }
            if (collision) continue;

            graph.nodePos[v] = { static_cast<float>(nr), static_cast<float>(nc), static_cast<float>(nz) };
            graph.layoutPositions[v] = { static_cast<float>(nc), static_cast<float>(nr) };
            q.push({ v, nr, nc });
        }
    }
    graph.layoutDirty = false;
}

} // namespace layout
