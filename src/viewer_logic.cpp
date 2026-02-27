#include "viewer_logic.h"
#include "analysis_logic.h"
#include "render/minimap_renderer.h"
#include <cstdio>
#include <cstdlib>
#include <functional>
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
#include <cctype>
#include <climits>

using namespace std;

namespace {
    std::function<void()> clearScreenFunc;
}

void initClearScreen() {
#ifdef _WIN32
    clearScreenFunc = [](){ system("cls"); };
#else
    clearScreenFunc = [](){ system("clear"); };
#endif
}

void clearScreen() {
    if (clearScreenFunc) {
        clearScreenFunc();
    }
}

void drawViewerMenu() {
    std::cout << "\n=== CBT Graph Viewer Menu ===\n";
    std::cout << "[↑↓←→/IJKL] Pan  [A] Add  [R] Remove  [F] Focus  [O] Unfocus  [T] Set Dist  [/] Search\n";
    std::cout << "[TAB] Cycle Focus [B] Book View [N] Next View [E] Page View [V] Page Cycle [Z] Zoom In   [X] Zoom Out\n";
    std::cout << "[M] Multi Foci Toggle\n";
    std::cout << "[G] Analytics  [D] DepthScale  [W] Weights  [S] Save  [U] Load  [ESC] Exit\n";
    std::cout << "==============================\n\n";
}

void renderNexusFlow(Graph& graph, NexusPhysicsState& physics) {
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
            // x = Column, y = Row
            physics.positions[node.index] = { static_cast<float>(rand() % DEFAULT_CONSOLE_WIDTH),
                                      static_cast<float>(rand() % DEFAULT_CONSOLE_HEIGHT) };
            physics.velocities[node.index] = { 0.0f, 0.0f };
        }
        physics.initialized = true;
        graph.needsLayoutReset = false;
    }

    // --- Force Calculation (run multiple iterations for stability) ---
    for (int i = 0; i < iterations; ++i) {
        std::map<int, Point2D> forces;

        // 1. Repulsive forces (all pairs)
        for (const auto& n1 : graph.nodes) {
            for (const auto& n2 : graph.nodes) {
                if (n1.index == n2.index) continue;

                float dx = physics.positions[n1.index].x - physics.positions[n2.index].x;
                float dy = physics.positions[n1.index].y - physics.positions[n2.index].y;
                float dist_sq = dx * dx + dy * dy;
                if (dist_sq < 1.0f) dist_sq = 1.0f; // Avoid division by zero

                float force = k_repel / dist_sq;
                forces[n1.index].x += dx * force;
                forces[n1.index].y += dy * force;
            }
        }

        // 2. Attractive forces (connected nodes)
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

        // 3. Update velocities and positions
        for (auto& node : graph.nodes) {
            physics.velocities[node.index].x = (physics.velocities[node.index].x + forces[node.index].x) * damping;
            physics.velocities[node.index].y = (physics.velocities[node.index].y + forces[node.index].y) * damping;
            physics.positions[node.index].x += physics.velocities[node.index].x;
            physics.positions[node.index].y += physics.velocities[node.index].y;

            // Clamp positions to screen boundaries (use default for NexusFlow for now as it's static)
            physics.positions[node.index].x = std::max(0.0f, std::min(static_cast<float>(physics.positions[node.index].x), static_cast<float>(DEFAULT_CONSOLE_WIDTH - 1)));
            physics.positions[node.index].y = std::max(0.0f, std::min(static_cast<float>(physics.positions[node.index].y), static_cast<float>(DEFAULT_CONSOLE_HEIGHT - 1)));
        }
    }


    // --- Rendering ---
    vector<string> screen(DEFAULT_CONSOLE_HEIGHT, string(DEFAULT_CONSOLE_WIDTH, ' '));

    // Render edges
    for (const auto& node : graph.nodes) {
        for (int neighbor_id : node.neighbors) {
             if (physics.positions.count(neighbor_id) == 0) continue;
            int r1 = physics.positions[node.index].y, c1 = physics.positions[node.index].x;
            int r2 = physics.positions[neighbor_id].y, c2 = physics.positions[neighbor_id].x;
            // (Bresenham's line algorithm - same as in renderGraph)
             int dr = abs(r2 - r1), dc = abs(c2 - c1);
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

    // Render nodes
    for (const auto& node : graph.nodes) {
        int r = physics.positions[node.index].y;
        int c = physics.positions[node.index].x;
        if (r >= 0 && r < DEFAULT_CONSOLE_HEIGHT && c >= 0 && c < DEFAULT_CONSOLE_WIDTH) {
             char glyph = (graph.isNodeFocused(node.index)) ? 'O' : 'X';
            screen[r][c] = glyph;
        }
    }

    clearScreen();
    cout << "=== CBT Graph Viewer (Nexus Flow) ===\n";
    for (const auto& row : screen) {
        cout << row << "\n";
    }
    cout << "[O: focused, X: node, .: edge]" << endl;
    drawViewerMenu();
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

void renderGraph(const Graph& graph, const ViewContext& view) {
    std::cout << "[DBG] panX=" << view.panX << "  panY=" << view.panY << "\n";
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
            float baseRow = (view.height - 1) / 2.0f;
            float baseCol = (view.width - 1) / 2.0f;
            pos[focus] = { baseRow, baseCol, 0.0f };
            q.push({ focus, static_cast<int>(baseRow), static_cast<int>(baseCol) });
        }
    } else {
        // Multi-focus: Distribute the focused nodes horizontally across the screen.
        const auto& focused_nodes = graph.focusedNodeIndices;
        int num_focused = focused_nodes.size();
        int spacing = view.width / (num_focused + 1); // e.g., 3 nodes -> space at 20, 40, 60
        int i = 1;
        for (int focus_id : focused_nodes) {
            if (graph.nodeExists(focus_id)) {
                float row = view.height / 2.0f;
                float col = static_cast<float>(i * spacing);
                pos[focus_id] = { row, col, 0.0f };
                q.push({ focus_id, static_cast<int>(row), static_cast<int>(col) });
                i++;
            }
        }
    }

    if (q.empty()) { // Fallback if no valid focus nodes were found or graph is empty
        std::cout << "No valid focus or nodes in graph. Skipping render.\n";
        return;
    }

    //int maxDist = graph.getMaxDistance(view.zoomLevel);
    int maxDist = view.maxRenderDistance;

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

            int parent_size = graph.calculateNodeSize(dz, view.zoomLevel);
            int child_size = graph.calculateNodeSize(nz, view.zoomLevel);
            int step = (parent_size / 2) + (child_size / 2) + Config::nodePadding + 2;
            int nr = wr + dr * step;
            int nc = wc + dc * step;

            // Bounding box collision check
            bool collision = false;
            for (const auto& [other_id, other_pos] : pos) {
                int other_node_size = graph.calculateNodeSize(other_pos.z, view.zoomLevel);
                // AABB check: if distance between centers is less than half the sum of sizes
                if (abs(nr - other_pos.x) * 2 < (child_size + other_node_size) &&
                    abs(nc - other_pos.y) * 2 < (child_size + other_node_size)) {
                    collision = true;
                    break;
                }
            }
            if (collision) continue;

            pos[v] = { static_cast<float>(nr), static_cast<float>(nc), static_cast<float>(nz) };
            q.push({ v, nr, nc });
        }
    }

    vector<string> screen(view.height, string(view.width, ' '));
    vector<vector<float>> zbuf(view.height, vector<float>(view.width, numeric_limits<float>::infinity()));

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
                if (r < 0 || r >= view.height || c < 0 || c >= view.width) continue;
                float depthVal = static_cast<float>(d);
                if (depthVal < zbuf[r][c]) {
                    screen[r][c] = glyph;
                    zbuf[r][c] = depthVal;
                }
            }
        }

        // After drawing the block:
        int labelRow = wr + size / 2 + view.panY + 1;
        int labelCol = wc + size / 2 + view.panX + 1;
/*         if (labelRow < view.height && labelCol < view.width) {
            const std::string& lbl = node.label;
            for (int i = 0; i < lbl.size(); ++i) {
                if (labelCol + i < view.width)
                    screen[labelRow][labelCol + i] = lbl[i];
            }
        } */
        // Only enter if row is within [0, H)
        if (labelRow >= 0 && labelRow < view.height) {
            const std::string& lbl = node.label;
            for (int i = 0; i < (int)lbl.size(); ++i) {
                int col = labelCol + i;
                // Guard both sides of the column index
                if (col >= 0 && col < view.width) {
                screen[labelRow][col] = lbl[i];
                }
            }
        }
    }

    for (const auto& [nid, coord] : pos) {
        for (int v : graph.nodeMap.at(nid).neighbors) {
            if (!pos.count(v)) continue;

            // Apply pan offset to the start and end points of the edge
            int r1 = static_cast<int>(coord.x) + view.panY, c1 = static_cast<int>(coord.y) + view.panX;
            int r2 = static_cast<int>(pos[v].x) + view.panY, c2 = static_cast<int>(pos[v].y) + view.panX;

            int dr = abs(r2 - r1), dc = abs(c2 - c1);
            int sr = (r1 < r2) ? 1 : -1;
            int sc = (c1 < c2) ? 1 : -1;
            int err = dr - dc;

            int rr = r1;
            int cc = c1;

            int maxSteps = std::max(dr, dc) + 1; // prevent infinite loop

            for (int step = 0; step < maxSteps; ++step) {
                if (rr >= 0 && rr < view.height && cc >= 0 && cc < view.width && screen[rr][cc] == ' ') {
                    screen[rr][cc] = '.';
                }
                if (rr == r2 && cc == c2) break;
                int e2 = 2 * err;
                if (e2 > -dc) { err -= dc; rr += sr; }
                if (e2 < dr)  { err += dr; cc += sc; }
            }
        }
    }

    clearScreen();
    cout << "=== CBT Graph Viewer (Full Layout) ===\n";
    for (const auto& row : screen) {
        cout << row << "\n";
    }
    cout << "[X: node  .: edge]" << endl;
    render::MinimapRenderer::render(graph, view);
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
    if (Config::quietMode) return;
    std::cout << "Enter node index to ADD as focus: ";
    int idx; std::cin >> idx;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    graph.addFocus(idx);
}

void promptFocusRemove(Graph& graph) {
    if (Config::quietMode) return;
    std::cout << "Enter focus index to REMOVE: ";
    int idx; std::cin >> idx;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    graph.removeFocus(idx);
}

void promptSetDistance(ViewContext& view) {
    if (Config::quietMode) return;
    std::cout << "Enter new max render distance: ";
    int d; std::cin >> d;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    view.maxRenderDistance = d;
}

void handleKeyPress(char key, input::ShortcutManager& shortcutManager) {
    shortcutManager.handleKey(std::toupper(key));
}

void renderBookView(Graph& graph, const ViewContext& view) {
    clearScreen();
    cout << "=== CBT Graph Viewer (Book View) ===\n";

    auto chapters = createBookStructure(graph, view);
    int row = 2; // Start rendering from the second row

    for (const auto& ch : chapters) {
        if (row >= view.height - 1) break;
        cout << "\n-- " << ch.chapterTitle << " (Depth " << ch.chapterDepth << ") --\n";
        row++;

        for (int nodeId : ch.nodeIds) {
            if (row >= view.height - 1) break;
            bool isFocused = graph.isNodeFocused(nodeId);
            cout << (isFocused ? " > " : "   ") << "[" << graph.nodeMap.at(nodeId).label << "]\n";
            row++;
        }
    }

    drawViewerMenu();
}

#include "console_logic.h"
#include <thread>
#include <chrono>

void runEditor(Graph& graph, bool runTests) {
    if (runTests) {
        std::cout << "Tests are now available via separate executables: unit_tests and bdd_tests\n";
        return;
    }

    init_terminal();
    initClearScreen();
    drawViewerMenu();

    NexusPhysicsState nexusPhysics;
    ViewContext view;
    input::CommandStack commandStack;
    input::ShortcutManager shortcutManager;

    // Register all standard shortcuts
    shortcutManager.registerShortcut('A', [&]() {
        int newIndex = 0;
        while (graph.nodeExists(newIndex)) newIndex++;
        commandStack.pushAndExecute(std::make_unique<input::AddNodeCommand>(graph, GraphNode("New", newIndex)));
    });
    shortcutManager.registerShortcut('R', [&]() {
        if (!graph.nodes.empty())
            commandStack.pushAndExecute(std::make_unique<input::RemoveNodeCommand>(graph, graph.nodes.back().index));
    });
    shortcutManager.registerShortcut('1', [&]() { commandStack.undo(); });
    shortcutManager.registerShortcut('2', [&]() { commandStack.redo(); });
    shortcutManager.registerShortcut('S', [&]() { saveGraphToCSV(graph, "graph_output.csv"); });
    shortcutManager.registerShortcut('U', [&]() { loadGraphFromCSV(graph, "graph_input.csv"); });
    shortcutManager.registerShortcut('G', [&]() { Config::viewerOverlayMode = !Config::viewerOverlayMode; });
    shortcutManager.registerShortcut('D', [&]() { Config::autoScaleDepth = !Config::autoScaleDepth; });
    shortcutManager.registerShortcut('W', [&]() { Config::showTopicWeights = !Config::showTopicWeights; });
    shortcutManager.registerShortcut('F', [&]() { promptFocusAdd(graph); });
    shortcutManager.registerShortcut('O', [&]() { promptFocusRemove(graph); });
    shortcutManager.registerShortcut('T', [&]() { promptSetDistance(view); });
    shortcutManager.registerShortcut('/', [&]() {
        std::cout << "Enter search keyword: ";
        std::string keyword;
        std::getline(std::cin, keyword);
        if (!keyword.empty()) {
            std::vector<int> matches = findSimilarTopics(graph, keyword);
            if (!matches.empty()) {
                graph.clearFocuses();
                for (int idx : matches) graph.addFocus(idx);
            }
        }
        graph.pause();
    });
    shortcutManager.registerShortcut('Z', [&]() { view.zoomIn(); });
    shortcutManager.registerShortcut('X', [&]() { view.zoomOut(); });
    shortcutManager.registerShortcut('I', [&]() { view.pan(0, -1); });
    shortcutManager.registerShortcut('K', [&]() { view.pan(0, 1); });
    shortcutManager.registerShortcut('J', [&]() { view.pan(-1, 0); });
    shortcutManager.registerShortcut('L', [&]() { view.pan(1, 0); });
    shortcutManager.registerShortcut('B', [&]() { view.currentViewMode = VM_BOOK_VIEW; });
    shortcutManager.registerShortcut('E', [&]() {
        std::cout << "Enter node index: ";
        int idx; std::cin >> idx; std::cin.ignore();
        renderNodePage(graph, idx);
    });
    shortcutManager.registerShortcut('M', [&]() { Config::allowMultiFocus = !Config::allowMultiFocus; });
    shortcutManager.registerShortcut('\t', [&]() { graph.cycleFocus(); });
    shortcutManager.registerShortcut('N', [&]() {
        int current = (static_cast<int>(view.currentViewMode) + 1) % (static_cast<int>(VM_COUNT));
        if (static_cast<ViewMode>(current) == VM_BOOK_VIEW) current = (current + 1) % VM_COUNT;
        view.currentViewMode = static_cast<ViewMode>(current);
        graph.needsLayoutReset = true;
    });

    while (true) {
        switch (view.currentViewMode) {
            case VM_NEXUS_FLOW:
                renderNexusFlow(graph, nexusPhysics);
                break;
            case VM_BOOK_VIEW:
                renderBookView(graph, view);
                break;
            case VM_PERSPECTIVE:
            default:
                renderGraph(graph, view);
                break;
        }
        if (Config::viewerOverlayMode) AnalyticsEngine::drawAnalyticsPanelOverlay(graph);

        int key = get_char_non_blocking();

        if (key != -1) {
            if (key == 27) { // ESC key
                int next_key = get_char_non_blocking();
                if (next_key == -1) { // A single ESC press
                    std::cout << "[Viewer] Exiting viewer mode\n";
                    break;
                }
                if (next_key == '[') {
                    int arrow_key = get_char_non_blocking();
                    switch (arrow_key) {
                        case 'A': view.pan(0, -1); break; // Up
                        case 'B': view.pan(0, 1); break;  // Down
                        case 'C': view.pan(1, 0); break;  // Right
                        case 'D': view.pan(-1, 0); break; // Left
                    }
                }
            } else {
                handleKeyPress(static_cast<char>(key), shortcutManager);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }
    restore_terminal();
}

bool executeGraphCommand(Graph& graph, ViewContext& view, const std::string& command) {
    static const std::unordered_map<std::string, std::function<void(Graph&, ViewContext&)>> command_handlers = {
        {"pan-up", [](Graph& g, ViewContext& v){ v.pan(0, -1); }},
        {"pan-down", [](Graph& g, ViewContext& v){ v.pan(0, 1); }},
        {"pan-left", [](Graph& g, ViewContext& v){ v.pan(-1, 0); }},
        {"pan-right", [](Graph& g, ViewContext& v){ v.pan(1, 0); }},
        {"zoom-in", [](Graph& g, ViewContext& v){ v.zoomIn(); }},
        {"zoom-out", [](Graph& g, ViewContext& v){ v.zoomOut(); }},
        {"cycle-focus", [](Graph& g, ViewContext& v){ g.cycleFocus(); }},
    };

    auto it = command_handlers.find(command);
    if (it != command_handlers.end()) {
        it->second(graph, view);
        std::cout << "Executed command: " << command << std::endl;
        return true;
    }

    std::cerr << "Error: Unknown internal command '" << command << "'" << std::endl;
    return false;
}



// Free helpers

// Book-based grouping
std::vector<BookChapter> createBookStructure(const Graph& g, const ViewContext& view) {
    std::map<std::string,BookChapter> chMap;
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
            int r = static_cast<int>(center.y) + dy;
            int c = static_cast<int>(center.x) + dx;
            if (r>=0 && r< (int)zbuf.size() &&
                c>=0 && c< (int)zbuf[0].size())
            {
                zbuf[r][c] = std::min(zbuf[r][c], depth);
            }
        }
    }
}
