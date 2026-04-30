#include "nexus_flow_view.h"
#include "../console_logic.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <map>

namespace layout {

void NexusFlowView::render(Graph& graph, NexusPhysicsState& physics, const SearchState& search) {
    // --- Constants for Force-Directed Layout ---
    const float k_repel = 2000.0f;  // Repulsive force strength
    const float k_attract = 0.05f; // Attractive force (spring)
    const float ideal_dist = 15.0f; // Ideal distance between connected nodes
    const float damping = 0.85f;    // Damping to prevent explosion
    const int iterations = 5;       // Iterations per frame for "flow"

    // --- Initialization ---
    if (!physics.initialized || graph.needsLayoutReset) {
        physics.positions.clear();
        physics.velocities.clear();
        for (const auto& node : graph.nodes) {
            physics.positions[node.index] = { static_cast<float>(rand() % DEFAULT_CONSOLE_WIDTH),
                                      static_cast<float>(rand() % DEFAULT_CONSOLE_HEIGHT) };
            physics.velocities[node.index] = { 0.0f, 0.0f };
        }
        physics.initialized = true;
        graph.needsLayoutReset = false;
    }

    // --- Force Calculation ---
    for (int i = 0; i < iterations; ++i) {
        std::map<int, Point2D> forces;

        for (const auto& n1 : graph.nodes) {
            for (const auto& n2 : graph.nodes) {
                if (n1.index == n2.index) continue;

                float dx = physics.positions[n1.index].x - physics.positions[n2.index].x;
                float dy = physics.positions[n1.index].y - physics.positions[n2.index].y;
                float dist_sq = dx * dx + dy * dy;
                if (dist_sq < 1.0f) dist_sq = 1.0f;

                float force = k_repel / dist_sq;
                forces[n1.index].x += dx * force;
                forces[n1.index].y += dy * force;
            }
        }

        for (const auto& node : graph.nodes) {
            for (int neighbor_id : node.neighbors) {
                if (physics.positions.count(neighbor_id) == 0) continue;

                float dx = physics.positions[neighbor_id].x - physics.positions[node.index].x;
                float dy = physics.positions[neighbor_id].y - physics.positions[node.index].y;
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
            physics.velocities[node.index].x = (physics.velocities[node.index].x + forces[node.index].x) * damping;
            physics.velocities[node.index].y = (physics.velocities[node.index].y + forces[node.index].y) * damping;
            physics.positions[node.index].x += physics.velocities[node.index].x;
            physics.positions[node.index].y += physics.velocities[node.index].y;

            physics.positions[node.index].x = std::max(0.0f, std::min(static_cast<float>(physics.positions[node.index].x), static_cast<float>(DEFAULT_CONSOLE_WIDTH - 1)));
            physics.positions[node.index].y = std::max(0.0f, std::min(static_cast<float>(physics.positions[node.index].y), static_cast<float>(DEFAULT_CONSOLE_HEIGHT - 1)));
        }
    }

    // --- Rendering ---
    std::vector<std::string> screen(DEFAULT_CONSOLE_HEIGHT, std::string(DEFAULT_CONSOLE_WIDTH, ' '));

    for (const auto& node : graph.nodes) {
        for (int neighbor_id : node.neighbors) {
             if (physics.positions.count(neighbor_id) == 0) continue;
            int r1 = physics.positions[node.index].y, c1 = physics.positions[node.index].x;
            int r2 = physics.positions[neighbor_id].y, c2 = physics.positions[neighbor_id].x;

            int dr = std::abs(r2 - r1), dc = std::abs(c2 - c1);
            int sr = (r1 < r2) ? 1 : -1;
            int sc = (c1 < c2) ? 1 : -1;
            int err = dr - dc;

            int rr = r1;
            int cc = c1;
            int maxSteps = std::max(dr, dc) + 1;

            for (int step = 0; step < maxSteps; ++step) {
                if (rr >= 0 && rr < DEFAULT_CONSOLE_HEIGHT && cc >= 0 && cc < DEFAULT_CONSOLE_WIDTH && screen[rr][cc] == ' ') {
                    screen[rr][cc] = '.';
                }
                if (rr == r2 && cc == c2) break;
                int e2 = 2 * err;
                if (e2 > -dc) { err -= dc; rr += sr; }
                if (e2 < dr)  { err += dr; cc += sc; }
            }
        }
    }

    for (const auto& node : graph.nodes) {
        int r = physics.positions[node.index].y;
        int c = physics.positions[node.index].x;
        if (r >= 0 && r < DEFAULT_CONSOLE_HEIGHT && c >= 0 && c < DEFAULT_CONSOLE_WIDTH) {
            char glyph = 'X';
            if (search.isActive && std::find(search.matches.begin(), search.matches.end(), node.index) != search.matches.end()) {
                glyph = (search.getActiveMatchNodeId() == node.index) ? 'S' : 's';
            } else if (graph.isNodeFocused(node.index)) {
                glyph = 'O';
            }
            screen[r][c] = glyph;
        }
    }

    // NOTE: clearScreen, Menu draw and Status bar should be handled by caller if we want to centralize UI
    // But for now keeping the original behavior where it prints directly.
    std::cout << "=== CBT Graph Viewer (Nexus Flow) ===\n";
    for (const auto& row : screen) {
        std::cout << row << "\n";
    }
    std::cout << "[O: focused, S/s: search, X: node, .: edge]" << std::endl;
}

} // namespace layout
