#ifndef VIEWER_LOGIC_H
#define VIEWER_LOGIC_H
#include "map_logic.h"
#include "console_logic.h"
#include "input/command_stack.h"
#include "input/shortcut_manager.h"

// How to draw each node block
enum DisplayMode {
    DM_BLOCK,      // solid 'X' squares
    DM_DOT,        // single '.' glyph
    DM_O,          // single 'O' glyph
    DM_TEXT_ONLY,  // no blocks, text labels only
    DM_COUNT
};

// Different visualization layouts
// Moved to map_logic.h to avoid circular dependencies

// Directional Panning Enum
enum class Direction { UP, DOWN, LEFT, RIGHT };

// Menu Rendering & Input Handling
void drawViewerMenu();                            // prints menu in terminal
void handleKeyPress(char key, input::ShortcutManager& shortcutManager);      // handles input from viewer loop

// Viewport Movement
void panView(Direction dir);                      // shifts viewer offset

// Analytics Overlay
void drawAnalyticsPanelOverlay(const Graph& g);   // draws full-screen overlay if enabled

// CSV Load/Save Trigger
bool saveGraphToCSV(const Graph& graph, const std::string& filename);  // serializes to disk
bool loadGraphFromCSV(Graph& graph, const std::string& filename);      // parses graph from file

// Viewer Engine (optional)
void runEditor(Graph& graph, bool runTests = false);                     // main viewer loop with menu+analytics
void renderGraph(const Graph& graph, const ViewContext& view);                   // renders visual graph content
void renderNexusFlow(Graph& graph, NexusPhysicsState& physics);               // renders force-directed layout
void renderBookView(Graph& graph, const ViewContext& view);                // renders book view layout

void promptFocusAdd(Graph& graph);
void promptFocusRemove(Graph& graph);
void promptSetDistance(ViewContext& view);

std::vector<BookChapter> createBookStructure(const Graph& g, const ViewContext& view);
int getGridLayer(int nodeIndex, int layerCount);
void renderNodePage(const Graph& graph, int nodeId);

// New function for executing commands from the CLI
bool executeGraphCommand(Graph& graph, ViewContext& view, const std::string& command);

#endif

