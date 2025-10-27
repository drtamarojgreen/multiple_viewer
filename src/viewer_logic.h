#ifndef VIEWER_LOGIC_H
#define VIEWER_LOGIC_H
#include "map_logic.h"
#include "console_logic.h"

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
void handleKeyPress(Graph& graph, char key);      // handles input from viewer loop

// Viewport Movement
void panView(Direction dir);                      // shifts viewer offset

// Analytics Overlay
void drawAnalyticsPanelOverlay(const Graph& g);   // draws full-screen overlay if enabled

// CSV Load/Save Trigger
bool saveGraphToCSV(const Graph& graph, const std::string& filename);  // serializes to disk
bool loadGraphFromCSV(Graph& graph, const std::string& filename);      // parses graph from file

// Viewer Engine (optional)
void runEditor(Graph& graph, bool runTests = false);                     // main viewer loop with menu+analytics
void renderGraph(const Graph& graph);                   // renders visual graph content
void renderNexusFlow(Graph& graph);               // renders force-directed layout
void renderBookView(Graph& graph);                // renders book view layout

void promptFocusAdd(Graph& graph);
void promptFocusRemove(Graph& graph);
void promptSetDistance(Graph& graph);

std::vector<BookChapter> createBookStructure(const Graph& g);
int getGridLayer(int nodeIndex, int layerCount);
void renderNodePage(const Graph& graph, int nodeId);

// New function for executing commands from the CLI
bool executeGraphCommand(Graph& graph, const std::string& command);

#endif

