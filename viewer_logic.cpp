#include "viewer_logic.h"
#include "file_logic.h"
#include "map_logic.h"
#include "search_logic.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <map>
#include <iomanip>
#include <queue>
#include <tuple>
#include <cmath>
#include <limits>
#include <unordered_map>
#include <functional>
#include <cctype>
#include <climits>

using namespace std;

void drawViewerMenu() {
    std::cout << "\n=== CBT Graph Viewer Menu ===\n";
    std::cout << "[↑↓←→] Pan  [A] Add  [R] Remove  [F] Focus  [O] Unfocus  [T] Set Dist  [/] Search\n";
    std::cout << "[TAB] Cycle Focus [B] Book View [E] Page View [V] Page Cycle [Z] Zoom In   [X] Zoom Out\n";
    std::cout << "[M] Multi Foci Toggle\n";
    std::cout << "[G] Analytics  [D] DepthScale  [W] Weights  [S] Save  [L] Load  [ESC] Exit\n";
    std::cout << "==============================\n\n";
}

void renderMindMap(const Graph& graph) {
    auto dist = graph.computeMultiFocusDistances();
    std::cout << "\n=== CBT Graph Render (Distance-X Map) ===\n";

    for (const auto& node : graph.nodes) {
        int d = dist.count(node.index) ? dist.at(node.index) : INT_MAX;
        std::cout << "\n[" << node.label << "] d=" << d << "\n";

        if (d <= 1) {
            for (int i = 0; i < 5; ++i) std::cout << "XXXXX\n";
        } else if (d <= 2) {
            for (int i = 0; i < 3; ++i) std::cout << "XXX\n";
        } else {
            std::cout << "X\n";
        }
    }
    std::cout << "\n";
}

void renderGraph(const Graph& graph) {
    std::cout << "[DBG] panX=" << graph.panX << "  panY=" << graph.panY << "\n";
    std::cout << "\n=== CBT Graph Render (Full Layout) ===\n";

    map<int, Coord3> pos;
    queue<tuple<int, int, int>> q;

    // --- Multi-Focus Layout Seeding ---
    // The goal is to place all focused nodes into the initial view.
    if (graph.focusedNodeIndices.empty() || graph.focusedNodeIndices.size() == 1) {
        // Handle single focus or no focus (default)
        int focus = -1;
        if (!graph.focusedNodeIndices.empty()) {
            focus = *graph.focusedNodeIndices.begin();
        } else if (!graph.nodes.empty()) {
            focus = graph.nodes.front().index; // Default to first node
        }

        if (graph.nodeExists(focus)) {
            int baseRow = (CONSOLE_HEIGHT - 1) / 2;
            int baseCol = (CONSOLE_WIDTH - 1) / 2;
            pos[focus] = { baseRow, baseCol, 0 };
            q.push({ focus, baseRow, baseCol });
        }
    } else {
        // Multi-focus: Distribute the focused nodes horizontally across the screen.
        const auto& focused_nodes = graph.focusedNodeIndices;
        int num_focused = focused_nodes.size();
        int spacing = CONSOLE_WIDTH / (num_focused + 1); // e.g., 3 nodes -> space at 20, 40, 60
        int i = 1;
        for (int focus_id : focused_nodes) {
            if (graph.nodeExists(focus_id)) {
                int row = CONSOLE_HEIGHT / 2;
                int col = i * spacing;
                pos[focus_id] = { row, col, 0 };
                q.push({ focus_id, row, col });
                i++;
            }
        }
    }

    if (q.empty()) { // Fallback if no valid focus nodes were found or graph is empty
        std::cout << "No valid focus or nodes in graph. Skipping render.\n";
        return;
    }

    //int maxDist = graph.getMaxDistance();
    int maxDist = graph.getMaxRenderDistance();

    while (!q.empty()) {
        auto [u, wr, wc] = q.front(); q.pop();
        int dz = pos[u].z;
        if (dz >= maxDist) continue;

        const auto& u_node = graph.nodeMap.at(u);
        const vector<pair<int, int>> directions = {
            {0, 1}, {1, 0}, {0, -1}, {-1, 0}, // Cardinal
            {1, 1}, {1, -1}, {-1, 1}, {-1, -1}  // Diagonal
        };
        int dir_idx = 0;

        for (int v : u_node.neighbors) {
            if (pos.count(v)) continue; // Already placed
            if (dir_idx >= (int)directions.size()) break; // Ran out of directions for this node

            auto [dr, dc] = directions[dir_idx++];
            int nz = dz + 1;
            if (nz > maxDist) continue;

            int size = graph.calculateNodeSize(nz);
            int step = size + Config::nodePadding + 3; // Increased step for more spacing
            int nr = wr + dr * step;
            int nc = wc + dc * step;

            // A simple check to avoid placing on top of another node in the same spot
            bool collision = false;
            for(const auto& [_, other_pos] : pos) {
                if (other_pos.x == nr && other_pos.y == nc) {
                    collision = true;
                    break;
                }
            }
            if (collision) continue;

            pos[v] = { nr, nc, nz };
            q.push({ v, nr, nc });
        }
    }

    vector<string> screen(CONSOLE_HEIGHT, string(CONSOLE_WIDTH, ' '));
    vector<vector<float>> zbuf(CONSOLE_HEIGHT, vector<float>(CONSOLE_WIDTH, numeric_limits<float>::infinity()));

    for (const auto& [nid, coord] : pos) {
        const auto& node = graph.nodeMap.at(nid);
        int d = coord.z;
        int wr = coord.x, wc = coord.y;
        int size = graph.calculateNodeSize(d);
        char glyph = (node.subjectIndex % 4 == 0 ? '@' :
                    node.subjectIndex % 4 == 1 ? '#' :
                    node.subjectIndex % 4 == 2 ? 'O' : 'X');

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                int r = wr + i - size / 2 + graph.panY;
                int c = wc + j - size / 2 + graph.panX;
                if (r < 0 || r >= CONSOLE_HEIGHT || c < 0 || c >= CONSOLE_WIDTH) continue;
                float depthVal = static_cast<float>(d);
                if (depthVal < zbuf[r][c]) {
                    screen[r][c] = glyph;
                    zbuf[r][c] = depthVal;
                }
            }
        }

        // After drawing the block:
        int labelRow = wr + size / 2 + graph.panY + 1;
        int labelCol = wc + size / 2 + graph.panX + 1;
/*         if (labelRow < CONSOLE_HEIGHT && labelCol < CONSOLE_WIDTH) {
            const std::string& lbl = node.label;
            for (int i = 0; i < lbl.size(); ++i) {
                if (labelCol + i < CONSOLE_WIDTH)
                    screen[labelRow][labelCol + i] = lbl[i];
            }
        } */
        // Only enter if row is within [0, H)
        if (labelRow >= 0 && labelRow < CONSOLE_HEIGHT) {
            const std::string& lbl = node.label;
            for (int i = 0; i < (int)lbl.size(); ++i) {
                int col = labelCol + i;
                // Guard both sides of the column index
                if (col >= 0 && col < CONSOLE_WIDTH) {
                screen[labelRow][col] = lbl[i];
                }
            }
        }
    }

    for (const auto& [nid, coord] : pos) {
        for (int v : graph.nodeMap.at(nid).neighbors) {
            if (!pos.count(v)) continue;

            // Apply pan offset to the start and end points of the edge
            int r1 = coord.x + graph.panY, c1 = coord.y + graph.panX;
            int r2 = pos[v].x + graph.panY, c2 = pos[v].y + graph.panX;

            int dr = abs(r2 - r1), dc = abs(c2 - c1);
            int sr = (r1 < r2) ? 1 : -1;
            int sc = (c1 < c2) ? 1 : -1;
            int err = dr - dc;

            int rr = r1;
            int cc = c1;

            int maxSteps = std::max(dr, dc) + 1; // prevent infinite loop

            for (int step = 0; step < maxSteps; ++step) {
                if (rr >= 0 && rr < CONSOLE_HEIGHT && cc >= 0 && cc < CONSOLE_WIDTH && screen[rr][cc] == ' ') {
                    screen[rr][cc] = '.';
                }
                if (rr == r2 && cc == c2) break;
                int e2 = 2 * err;
                if (e2 > -dc) { err -= dc; rr += sr; }
                if (e2 < dr)  { err += dr; cc += sc; }
            }
        }
    }

    system("cls");
    cout << "=== CBT Graph Viewer (Full Layout) ===\n";
    for (const auto& row : screen) {
        cout << row << "\n";
    }
    cout << "[X: node  .: edge]" << endl;
    drawViewerMenu();
}

void panView(Direction dir) {
    switch (dir) {
        case Direction::UP:    std::cout << "[Pan] Up\n"; break;
        case Direction::DOWN:  std::cout << "[Pan] Down\n"; break;
        case Direction::LEFT:  std::cout << "[Pan] Left\n"; break;
        case Direction::RIGHT: std::cout << "[Pan] Right\n"; break;
    }
}

void promptFocusAdd(Graph& graph) {
    std::cout << "Enter node index to ADD as focus: ";
    int idx; std::cin >> idx;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    graph.addFocus(idx);
}

void promptFocusRemove(Graph& graph) {
    std::cout << "Enter focus index to REMOVE: ";
    int idx; std::cin >> idx;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    graph.removeFocus(idx);
}

void promptSetDistance(Graph& graph) {
    std::cout << "Enter new max render distance: ";
    int d; std::cin >> d;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    graph.setMaxRenderDistance(d);
}

void handleKeyPress(Graph& graph, char key) {
    static const std::unordered_map<char, std::function<void()>> handlers = {
        {'A', [&]() {
            int newIndex = int(graph.nodes.size());
            GraphNode n{"New", newIndex, {}, 1, newIndex - 1};
            graph.addNode(n);
        }},
        {'R', [&]() {
            if (!graph.nodes.empty())
                graph.removeNode(graph.nodes.back().index);
        }},
        {'S', [&]() { saveGraphToCSV(graph, "graph_output.csv"); }},
        {'L', [&]() { loadGraphFromCSV(graph, "graph_input.csv"); }},
        {'G', [&]() { Config::viewerOverlayMode = !Config::viewerOverlayMode; }},
        {'D', [&]() { Config::autoScaleDepth = !Config::autoScaleDepth; }},
        {'W', [&]() { Config::showTopicWeights = !Config::showTopicWeights; }},
        {'F', [&]() { promptFocusAdd(graph); }},
        {'O', [&]() { promptFocusRemove(graph); }},
        {'T', [&]() { promptSetDistance(graph); }},
        {'/', [&]() {
            std::cout << "Enter search keyword: ";
            std::string keyword;
            std::getline(std::cin, keyword);

            if (keyword.empty()) {
                std::cout << "[Search] Empty keyword\n";
                return;
            }

            std::vector<int> matches = findSimilarTopics(graph, keyword);

            if (matches.empty()) {
                std::cout << "[Search] No matches found for \"" << keyword << "\"\n";
            } else {
                std::cout << "[Search] Found " << matches.size() << " match(es):\n";
                for (int idx : matches)
                    std::cout << "  • [" << graph.nodeMap.at(idx).label << "] (id=" << idx << ")\n";

                graph.clearFocuses();
                for (int idx : matches)
                    graph.addFocus(idx);
                std::cout << "[Search] Focused matching nodes.\n";
            }

            graph.pause();
        }},
        {'Z', [&]() { graph.zoomIn(); }},
        {'X', [&]() { graph.zoomOut(); }},
        {'I', [&]() { graph.pan(0, -1); }}, // Up
        {'K', [&]() { graph.pan(0, 1); }},  // Down
        {'J', [&]() { graph.pan(-1, 0); }}, // Left
        {'L', [&]() { graph.pan(1, 0); }},  // Right
        {'B', [&]() {
            std::cout << "\n=== CBT Book View ===\n";
            auto chapters = createBookStructure(graph);
            for (const auto& ch : chapters) {
                std::cout << "\n-- " << ch.chapterTitle << " (Depth " << ch.chapterDepth << ") --\n";
                for (int nodeId : ch.nodeIds) {
                    std::cout << "[" << graph.nodeMap.at(nodeId).label << "]\n";
                }
            }
            graph.pause();
        }},
        {'E', [&]() {
            std::cout << "Enter node index to view page: ";
            int idx; std::cin >> idx;
            std::cin.ignore();
            renderNodePage(graph, idx);
        }},
        {'V', [&]() {
            if (graph.focusedNodeIndex >= 0)
                renderNodePage(graph, graph.focusedNodeIndex);
            else
                std::cout << "[Page] No focused node selected.\n";
        }},
        {'M', [&]() { Config::allowMultiFocus = !Config::allowMultiFocus; }},
        {'[', [&]() { graph.cycleFocus(); }},
        {'\t', [&]() { graph.cycleFocus(); }}

 };

    key = std::toupper(key);
    auto it = handlers.find(key);
    if (it != handlers.end()) {
        it->second();
    } else {
        std::cout << "[Viewer] Unknown key: " << key << "\n";
    }
}

void runEditor(Graph& graph) {
    drawViewerMenu();

    while (true) {
        renderGraph(graph);
        //renderMindMap(graph);
        if (Config::viewerOverlayMode) drawAnalyticsPanelOverlay(graph);
        int key = std::cin.get(); // Use int to handle special key codes
        //key = std::toupper(key);  // Allow both 'a' and 'A' for example
        if (key == 27) { // ESC key always exits
            std::cout << "[Viewer] Exiting viewer mode\n";
            break;
        } else {
            handleKeyPress(graph, static_cast<char>(key));
        }
    }
}


// Adaptive label length
int Graph::getAdaptiveLabelLength(int depth) const {
    int base = getMaxLabelLength();
    float factor = (depth==0 ? 1.5f : (depth==1 ? 1.0f : 0.5f));
    return std::max(3, int(base * factor));
}

// subject filter
bool Graph::passesSubjectFilter(int nodeId) const {
    if (!subjectFilterOnly) return true;
    if (focusedNodeIndex<0)   return true;
    return nodes.at(nodeId).subjectIndex
         == nodes.at(focusedNodeIndex).subjectIndex;
}

// focus-only mode
bool Graph::isFocusOnlyView() const {
    return focusOnlyAtMaxZoom && zoomLevel == ZoomLevel::Z5;
}

// continuous zoom sizing
int Graph::calculateNodeSize(int depth) const {
    // Turn zoomLevel Z1-Z5 (internally 0-4) into a size from 1-5.
    // This ensures node size is affected by zoom but capped at 5x5.
    // The depth parameter is currently ignored for a simpler sizing model.
    return static_cast<int>(zoomLevel) + 1;
}

// proximity depth
float Graph::getProximityDepth(int nodeId) const {
    auto it = nodePos.find(nodeId);
    if (it == nodePos.end()) return 1.0f;
    float cx = 80.0f / 2.0f, cy = 25.0f / 2.0f;
    float dx = it->second.y - cx;
    float dy = it->second.x - cy;
    float dist = std::sqrt(dx*dx + dy*dy);
    return std::min(1.0f, dist / std::max(cx, cy));
}

// Free helpers

// Book-based grouping
std::vector<BookChapter> createBookStructure(const Graph& g) {
    std::map<std::string,BookChapter> chMap;
    for (const auto& node : g.nodes) {
        int depth = (g.nodePos.count(node.index)
                   ? g.nodePos.at(node.index).z : 0);

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

void renderNodePage(const Graph& graph, int nodeId) {
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
    std::cout << "Parent: " << n.subjectIndex << "\n";
    std::cout << "Neighbors:\n";
    for (int nbr : n.neighbors)
        std::cout << "  • " << graph.nodeMap.at(nbr).label << "\n";

    graph.pause();
}

// Grid layer assignment
int getGridLayer(int nodeIndex, int layerCount) {
    return (layerCount>0 ? nodeIndex % layerCount : 0);
}


// Z-buffer pixel update
void updateZBuffer(std::vector<std::vector<float>>& zbuf,
                   Point2D center, int size, float depth) {
    int half = size/2;
    for (int dy=-half; dy<=half; ++dy) {
        for (int dx=-half; dx<=half; ++dx) {
            int r = center.y + dy;
            int c = center.x + dx;
            if (r>=0 && r< (int)zbuf.size() &&
                c>=0 && c< (int)zbuf[0].size())
            {
                zbuf[r][c] = std::min(zbuf[r][c], depth);
            }
        }
    }
}