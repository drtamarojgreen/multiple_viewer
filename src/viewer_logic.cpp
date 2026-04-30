#include "viewer_logic.h"
#include "analysis_logic.h"
#include "layout/minimap_renderer.h"
#include "layout/nexus_flow_view.h"
#include "layout/book_view.h"
#include "layout/page_view.h"
#include "render/renderer_factory.h"
#include "layout/layout_manager.h"
#include "ui/main_menu.h"
#include <cstdio>
#include <cstdlib>
#include <functional>
#include "io/io_manager.h"
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
#include "console_logic.h"
#include <thread>

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

void drawStatusBar(const SearchState& search) {
    if (search.isActive) {
        std::cout << " SEARCH: " << search.keyword << "_";
        if (search.lastSearchHadNoResults) {
            std::cout << " [No Results]";
        } else if (!search.matches.empty()) {
            std::cout << " [" << (search.activeMatchIndex + 1) << "/" << search.matches.size() << "]";
        }
    } else {
        std::cout << " [Mode: Navigation]";
    }
    std::cout << "\n\n";
}

void renderGraph(const Graph& graph, const ViewContext& view, const SearchState& search) {
    std::cout << "[DBG] panX=" << view.panX << "  panY=" << view.panY << "\n";
    std::cout << "\n=== CBT Graph Render (Full Layout) ===\n";

    vector<string> screen(view.height, string(view.width, ' '));
    vector<vector<float>> zbuf(view.height, vector<float>(view.width, numeric_limits<float>::infinity()));

    for (const auto& [nid, coord] : graph.nodePos) {
        const auto& node = graph.nodeMap.at(nid);
        int d = static_cast<int>(coord.z);
        int wr = static_cast<int>(coord.x), wc = static_cast<int>(coord.y);
        int size = graph.calculateNodeSize(d, view.zoomLevel);
        char glyph = (node.subjectIndex % 4 == 0 ? '@' :
                    node.subjectIndex % 4 == 1 ? '#' :
                    node.subjectIndex % 4 == 2 ? 'O' : 'X');

        if (search.isActive && std::find(search.matches.begin(), search.matches.end(), node.index) != search.matches.end()) {
            glyph = (search.getActiveMatchNodeId() == node.index) ? 'S' : 's';
        }

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

        int labelRow = wr + size / 2 + view.panY + 1;
        int labelCol = wc + size / 2 + view.panX + 1;
        if (labelRow >= 0 && labelRow < view.height) {
            const std::string& lbl = node.label;
            for (int i = 0; i < (int)lbl.size(); ++i) {
                int col = labelCol + i;
                if (col >= 0 && col < view.width) {
                    screen[labelRow][col] = lbl[i];
                }
            }
        }
    }

    for (const auto& [nid, coord] : graph.nodePos) {
        for (int v : graph.nodeMap.at(nid).neighbors) {
            if (!graph.nodePos.count(v)) continue;

            int r1 = static_cast<int>(coord.x) + view.panY, c1 = static_cast<int>(coord.y) + view.panX;
            int r2 = static_cast<int>(graph.nodePos.at(v).x) + view.panY, c2 = static_cast<int>(graph.nodePos.at(v).y) + view.panX;

            int dr = abs(r2 - r1), dc = abs(c2 - c1);
            int sr = (r1 < r2) ? 1 : -1;
            int sc = (c1 < c2) ? 1 : -1;
            int err = dr - dc;

            int rr = r1;
            int cc = c1;

            int maxSteps = std::max(dr, dc) + 1;

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
    cout << "[S/s: search results, X/@/#/O: nodes, .: edge]" << endl;
    render::MinimapRenderer::render(graph, view);
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

void runEditor(Graph& graph, bool runTests) {
    if (runTests) {
        std::cout << "Tests are now available via separate executables: unit_tests and bdd_tests\n";
        return;
    }

    init_terminal();
    initClearScreen();

    NexusPhysicsState nexusPhysics;
    ViewContext view;
    input::CommandStack commandStack;
    input::ShortcutManager shortcutManager;
    SearchState searchState;

    auto renderer = render::RendererFactory::createRenderer(render::RendererType::CONSOLE);
    renderer->initialize(view.width, view.height + 10);

    shortcutManager.registerShortcut('A', "Add Node", [&]() {
        int newIndex = 0;
        while (graph.nodeExists(newIndex)) newIndex++;
        commandStack.pushAndExecute(std::make_unique<input::AddNodeCommand>(graph, GraphNode("New", newIndex)));
    });
    shortcutManager.registerShortcut('R', "Remove Node", [&]() {
        if (!graph.nodes.empty())
            commandStack.pushAndExecute(std::make_unique<input::RemoveNodeCommand>(graph, graph.nodes.back().index));
    });
    shortcutManager.registerShortcut('1', "Undo", [&]() { commandStack.undo(); });
    shortcutManager.registerShortcut('2', "Redo", [&]() { commandStack.redo(); });
    shortcutManager.registerShortcut('S', "Save CSV", [&]() { io::IOManager::saveGraphToCSV(graph, "graph_output.csv"); });
    shortcutManager.registerShortcut('U', "Load CSV", [&]() { io::IOManager::loadGraphFromCSV(graph, "graph_input.csv"); });
    shortcutManager.registerShortcut('G', "Toggle Analytics", [&]() { Config::viewerOverlayMode = !Config::viewerOverlayMode; });
    shortcutManager.registerShortcut('D', "Toggle AutoScale Depth", [&]() { Config::autoScaleDepth = !Config::autoScaleDepth; });
    shortcutManager.registerShortcut('W', "Toggle Weights", [&]() { Config::showTopicWeights = !Config::showTopicWeights; });
    shortcutManager.registerShortcut('F', "Add Focus", [&]() {
        renderer->setStatusMessage("Enter node index to ADD as focus: ");
        renderer->present();
        int idx; std::cin >> idx; std::cin.ignore(1000, '\n');
        graph.addFocus(idx);
        renderer->setStatusMessage("Focused node " + std::to_string(idx));
    });
    shortcutManager.registerShortcut('O', "Remove Focus", [&]() {
        renderer->setStatusMessage("Enter focus index to REMOVE: ");
        renderer->present();
        int idx; std::cin >> idx; std::cin.ignore(1000, '\n');
        graph.removeFocus(idx);
        renderer->setStatusMessage("Unfocused node " + std::to_string(idx));
    });
    shortcutManager.registerShortcut('T', "Set Dist", [&]() {
        renderer->setStatusMessage("Enter new max render distance: ");
        renderer->present();
        int d; std::cin >> d; std::cin.ignore(1000, '\n');
        view.maxRenderDistance = d;
    });
    shortcutManager.registerShortcut('/', "Search", [&]() {
        renderer->setStatusMessage("Enter search keyword: ");
        renderer->present();
        std::string keyword;
        std::getline(std::cin, keyword);
        if (!keyword.empty()) {
            std::vector<int> matches = findSimilarTopics(graph, keyword);
            if (!matches.empty()) {
                graph.clearFocuses();
                for (int idx : matches) graph.addFocus(idx);
                renderer->setStatusMessage("Found " + std::to_string(matches.size()) + " matches.");
            } else {
                renderer->setStatusMessage("No matches found.");
            }
        }
        searchState.isActive = true;
    });
    shortcutManager.registerShortcut('Z', "Zoom In", [&]() { view.zoomIn(); });
    shortcutManager.registerShortcut('X', "Zoom Out", [&]() { view.zoomOut(); });
    shortcutManager.registerShortcut('I', "Pan Up", [&]() { view.pan(0, -1); });
    shortcutManager.registerShortcut('K', "Pan Down", [&]() { view.pan(0, 1); });
    shortcutManager.registerShortcut('J', "Pan Left", [&]() { view.pan(-1, 0); });
    shortcutManager.registerShortcut('L', "Pan Right", [&]() { view.pan(1, 0); });
    shortcutManager.registerShortcut('B', "Book View", [&]() { view.currentViewMode = VM_BOOK_VIEW; });
    shortcutManager.registerShortcut('E', "Page View", [&]() {
        renderer->setStatusMessage("Enter node index for Page View: ");
        renderer->present();
        int idx; std::cin >> idx; std::cin.ignore();
        layout::PageView::renderNodePage(graph, idx);
    });
    shortcutManager.registerShortcut('M', "Multi Foci Toggle", [&]() { Config::allowMultiFocus = !Config::allowMultiFocus; });
    shortcutManager.registerShortcut('H', "Help Toggle", [&]() { view.showHelp = !view.showHelp; });
    shortcutManager.registerShortcut('\t', "Cycle Focus", [&]() { graph.cycleFocus(); });
    shortcutManager.registerShortcut('N', "Next View", [&]() {
        int current = (static_cast<int>(view.currentViewMode) + 1) % (static_cast<int>(VM_COUNT));
        if (static_cast<ViewMode>(current) == VM_BOOK_VIEW) current = (current + 1) % VM_COUNT;
        view.currentViewMode = static_cast<ViewMode>(current);
        graph.needsLayoutReset = true;
    });

    while (true) {
        if (view.currentViewMode == VM_PERSPECTIVE) {
            layout::LayoutManager::applyPerspectiveBFS(graph, view);
            renderGraph(graph, view, searchState);
        } else if (view.currentViewMode == VM_NEXUS_FLOW) {
            layout::LayoutManager::applyForceDirected(graph, view);
            clearScreen();
            layout::NexusFlowView::render(graph, nexusPhysics, searchState);
        } else if (view.currentViewMode == VM_BOOK_VIEW) {
            clearScreen();
            layout::BookView::render(graph, view, searchState);
        }

        ui::MainMenu::draw(shortcutManager);
        drawStatusBar(searchState);

        if (Config::viewerOverlayMode) AnalyticsEngine::drawAnalyticsPanelOverlay(graph);

        int key = get_char_non_blocking();

        if (key != -1) {
            if (searchState.isActive) {
                if (key == 27) {
                    int next_key = get_char_non_blocking();
                    if (next_key == -1) {
                        searchState.isActive = false;
                    } else if (next_key == '[') {
                        int arrow_key = get_char_non_blocking();
                        if (!searchState.matches.empty()) {
                            if (arrow_key == 'A' || arrow_key == 'D') {
                                searchState.activeMatchIndex = (searchState.activeMatchIndex - 1 + searchState.matches.size()) % searchState.matches.size();
                            } else if (arrow_key == 'B' || arrow_key == 'C') {
                                searchState.activeMatchIndex = (searchState.activeMatchIndex + 1) % searchState.matches.size();
                            }
                            if (searchState.activeMatchIndex != -1) {
                                graph.clearFocuses();
                                graph.addFocus(searchState.matches[searchState.activeMatchIndex]);
                            }
                        }
                    }
                } else if (key == 10 || key == 13) {
                    searchState.isActive = false;
                } else if (key == 127 || key == 8) {
                    if (!searchState.keyword.empty()) {
                        searchState.keyword.pop_back();
                        searchState.matches = findSimilarTopics(graph, searchState.keyword);
                        searchState.lastSearchHadNoResults = !searchState.keyword.empty() && searchState.matches.empty();
                        searchState.activeMatchIndex = searchState.matches.empty() ? -1 : 0;
                    }
                } else if (isprint(key)) {
                    searchState.keyword += static_cast<char>(key);
                    searchState.matches = findSimilarTopics(graph, searchState.keyword);
                    searchState.lastSearchHadNoResults = searchState.matches.empty();
                    searchState.activeMatchIndex = searchState.matches.empty() ? -1 : 0;

                    if (!searchState.matches.empty()) {
                        graph.clearFocuses();
                        graph.addFocus(searchState.matches[0]);
                    }
                }
            } else {
                if (key == 27) {
                    int next_key = get_char_non_blocking();
                    if (next_key == -1) {
                        break;
                    }
                    if (next_key == '[') {
                        int arrow_key = get_char_non_blocking();
                        switch (arrow_key) {
                            case 'A': view.pan(0, -1); break;
                            case 'B': view.pan(0, 1); break;
                            case 'C': view.pan(1, 0); break;
                            case 'D': view.pan(-1, 0); break;
                        }
                    }
                } else {
                    char c = static_cast<char>(std::toupper(key));
                    shortcutManager.handleKey(c);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
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

int getGridLayer(int nodeIndex, int layerCount) {
    return (layerCount>0 ? nodeIndex % layerCount : 0);
}
