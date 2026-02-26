#include "console_renderer.h"
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <tuple>
#include <limits>
#include <algorithm>
#include <map>

namespace render {

bool ConsoleRenderer::initialize(int width, int height) {
    width_ = width;
    height_ = height;
    return true;
}

void ConsoleRenderer::clear() {
    // For console, we often clear by printing empty lines or system("clear")
    // In this app, it's done at the start of renderGraph usually.
}

void ConsoleRenderer::render(const Graph& graph, const ViewContext& view) {
    // Logic extracted from viewer_logic.cpp's renderGraph
    std::map<int, Coord3> pos;
    std::queue<std::tuple<int, int, int>> q;

    if (graph.focusedNodeIndices.empty() || graph.focusedNodeIndices.size() == 1) {
        int focus = -1;
        if (!graph.focusedNodeIndices.empty()) {
            focus = *graph.focusedNodeIndices.begin();
        } else if (!graph.nodes.empty()) {
            focus = graph.nodes.front().index;
        }

        if (graph.nodeExists(focus)) {
            float baseRow = (height_ - 1) / 2.0f;
            float baseCol = (width_ - 1) / 2.0f;
            pos[focus] = { baseRow, baseCol, 0.0f };
            q.push({ focus, static_cast<int>(baseRow), static_cast<int>(baseCol) });
        }
    } else {
        const auto& focused_nodes = graph.focusedNodeIndices;
        int num_focused = focused_nodes.size();
        int spacing = width_ / (num_focused + 1);
        int i = 1;
        for (int focus_id : focused_nodes) {
            if (graph.nodeExists(focus_id)) {
                float row = height_ / 2.0f;
                float col = static_cast<float>(i * spacing);
                pos[focus_id] = { row, col, 0.0f };
                q.push({ focus_id, static_cast<int>(row), static_cast<int>(col) });
                i++;
            }
        }
    }

    if (q.empty()) return;

    int maxDist = view.maxRenderDistance;

    while (!q.empty()) {
        auto [u, wr, wc] = q.front(); q.pop();
        int dz = pos[u].z;
        if (dz >= maxDist) continue;

        const auto& u_node = graph.nodeMap.at(u);
        const std::vector<std::pair<int, int>> directions = {
            {0, 1}, {1, 0}, {0, -1}, {-1, 0},
            {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
        };
        int dir_idx = 0;

        for (int v : u_node.neighbors) {
            if (pos.count(v)) continue;
            if (dir_idx >= (int)directions.size()) break;

            auto [dr, dc] = directions[dir_idx++];
            int nz = dz + 1;
            if (nz > maxDist) continue;

            int parent_size = graph.calculateNodeSize(dz, view.zoomLevel);
            int child_size = graph.calculateNodeSize(nz, view.zoomLevel);
            int step = (parent_size / 2) + (child_size / 2) + 1 + 2; // Node padding hardcoded for now or use Config
            int nr = wr + dr * step;
            int nc = wc + dc * step;

            bool collision = false;
            for (const auto& [other_id, other_pos] : pos) {
                int other_node_size = graph.calculateNodeSize(other_pos.z, view.zoomLevel);
                if (std::abs(nr - other_pos.x) * 2 < (child_size + other_node_size) &&
                    std::abs(nc - other_pos.y) * 2 < (child_size + other_node_size)) {
                    collision = true;
                    break;
                }
            }
            if (collision) continue;

            pos[v] = { static_cast<float>(nr), static_cast<float>(nc), static_cast<float>(nz) };
            q.push({ v, nr, nc });
        }
    }

    std::vector<std::string> screen(height_, std::string(width_, ' '));
    std::vector<std::vector<float>> zbuf(height_, std::vector<float>(width_, std::numeric_limits<float>::infinity()));

    for (const auto& [nid, coord] : pos) {
        const auto& node = graph.nodeMap.at(nid);
        int d = static_cast<int>(coord.z);
        int wr = static_cast<int>(coord.x), wc = static_cast<int>(coord.y);
        int size = graph.calculateNodeSize(d, view.zoomLevel);
        char glyph = (node.subjectIndex % 4 == 0 ? '@' :
                    node.subjectIndex % 4 == 1 ? '#' :
                    node.subjectIndex % 4 == 2 ? 'O' : 'X');

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                int r = wr + i - size / 2 + view.panY;
                int c = wc + j - size / 2 + view.panX;
                if (r < 0 || r >= height_ || c < 0 || c >= width_) continue;
                float depthVal = static_cast<float>(d);
                if (depthVal < zbuf[r][c]) {
                    screen[r][c] = glyph;
                    zbuf[r][c] = depthVal;
                }
            }
        }

        int labelRow = wr + size / 2 + view.panY + 1;
        int labelCol = wc + size / 2 + view.panX + 1;
        if (labelRow >= 0 && labelRow < height_) {
            const std::string& lbl = node.label;
            for (int i = 0; i < (int)lbl.size(); ++i) {
                int col = labelCol + i;
                if (col >= 0 && col < width_) {
                    screen[labelRow][col] = lbl[i];
                }
            }
        }
    }

    for (const auto& [nid, coord] : pos) {
        for (int v : graph.nodeMap.at(nid).neighbors) {
            if (!pos.count(v)) continue;

            int r1 = static_cast<int>(coord.x) + view.panY, c1 = static_cast<int>(coord.y) + view.panX;
            int r2 = static_cast<int>(pos[v].x) + view.panY, c2 = static_cast<int>(pos[v].y) + view.panX;

            int dr = std::abs(r2 - r1), dc = std::abs(c2 - c1);
            int sr = (r1 < r2) ? 1 : -1;
            int sc = (c1 < c2) ? 1 : -1;
            int err = dr - dc;

            int rr = r1;
            int cc = c1;

            int maxSteps = std::max(dr, dc) + 1;

            for (int step = 0; step < maxSteps; ++step) {
                if (rr >= 0 && rr < height_ && cc >= 0 && cc < width_ && screen[rr][cc] == ' ') {
                    screen[rr][cc] = '.';
                }
                if (rr == r2 && cc == c2) break;
                int e2 = 2 * err;
                if (e2 > -dc) { err -= dc; rr += sr; }
                if (e2 < dr)  { err += dr; cc += sc; }
            }
        }
    }

    // Print to stdout
    // In a real multi-backend system, we might buffer this or use a clearScreen call
    std::cout << "\n=== CBT Graph Viewer (Full Layout) ===\n";
    for (const auto& row : screen) {
        std::cout << row << "\n";
    }
}

void ConsoleRenderer::present() {
    // No-op for console usually, or we could handle the final flush here.
}

void ConsoleRenderer::shutdown() {
}

} // namespace render
