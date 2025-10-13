# Project Requirements (Phase 1)

This document outlines the initial requirements for the CBT Graph Viewer application.

## Core Features
- **Graph Loading/Saving:** The application must be able to load a graph structure from a CSV file and save the current state back to a CSV file.
- **Graph Rendering:** The graph should be rendered in a 2D layout within the console window.
- **Node Representation:** Nodes should be visually distinct.
- **Edge Representation:** Connections between nodes should be drawn.

## User Interaction
- **Panning:** The user must be able to pan the view (up, down, left, right).
- **Zooming:** The user must be able to zoom in and out, affecting the visible detail or area of the graph.
- **Focus:** The user must be able to select or "focus" on a specific node.
- **Focus Cycling:** The user should be able to cycle the focus to the next node in the graph using the `Tab` key.

## Future Considerations
- Multi-focus support
- Advanced analytics panel
- Different layout algorithms (e.g., "Book View")

# Detailed Requirements for Console-Based Graph Viewer/Editor

## 1. Graph Loading & Saving

1. **Load graph from CSV file**
   - Function: `bool loadGraphFromCSV(const std::string& filename);`
   - Variables: `std::vector<Node> nodes; std::vector<Edge> edges;`
   - Constraints: Handle malformed files, duplicate nodes/edges, missing fields.

2. **Save graph to CSV file**
   - Function: `bool saveGraphToCSV(const std::string& filename);`
   - Variables: `nodes`, `edges`
   - Constraints: Ensure file is writable, handle I/O errors.

3. **Validate graph structure after loading**
   - Function: `bool validateGraph();`
   - Variables: `nodes`, `edges`
   - Constraints: No orphaned edges, unique node IDs.

## 2. Graph Data Structures

4. **Node representation**
   - Struct: `struct Node { int id; std::string label; int x, y; };`
   - Constraints: Unique `id`, valid coordinates.

5. **Edge representation**
   - Struct: `struct Edge { int from; int to; double weight; };`
   - Constraints: `from` and `to` must exist in `nodes`.

6. **Graph container**
   - Class: `class Graph { std::vector<Node> nodes; std::vector<Edge> edges; };`
   - Constraints: Efficient lookup and modification.

## 3. Rendering

7. **Render graph in 2D console**
   - Function: `void renderGraph(const Graph& graph);`
   - Variables: `nodes`, `edges`, `screenBuffer[height][width]`
   - Constraints: Handle overlapping nodes, fit to console size.

8. **Render nodes visually distinct**
   - Function: `void renderNode(const Node& node, char symbol);`
   - Variables: `symbol`
   - Constraints: Avoid symbol collisions.

9. **Render edges**
   - Function: `void renderEdge(const Edge& edge);`
   - Variables: `edge`
   - Constraints: Draw lines between node coordinates.

10. **Clear screen before redraw**
    - Function: `void clearScreen();`
    - Constraints: Platform-specific (e.g., Windows vs. Unix).

## 4. User Interaction

11. **Pan view**
    - Function: `void panView(Direction dir);`
    - Variables: `int offsetX, offsetY;`
    - Constraints: Prevent panning out of bounds.

12. **Zoom in/out**
    - Function: `void zoomIn(); void zoomOut();`
    - Variables: `int zoomLevel;`
    - Constraints: Clamp zoom level to min/max.

13. **Select/focus node**
    - Function: `void focusNode(int nodeId);`
    - Variables: `int focusedNodeId;`
    - Constraints: Node must exist.

14. **Cycle focus**
    - Function: `void cycleFocus();`
    - Variables: `std::vector<int> focusOrder;`
    - Constraints: Wrap around at end.

15. **Multi-focus support**
    - Function: `void addFocus(int nodeId); void removeFocus(int nodeId);`
    - Variables: `std::set<int> focusedNodeIds;`
    - Constraints: No duplicates, max focus count.

16. **Unfocus all**
    - Function: `void clearFocus();`
    - Variables: `focusedNodeIds`
    - Constraints: N/A

17. **Keyboard input handling**
    - Function: `void handleKeyPress(char key);`
    - Variables: `key`
    - Constraints: Map keys to actions.

18. **Show node details**
    - Function: `void showNodeDetails(int nodeId);`
    - Variables: `Node`
    - Constraints: Node must exist.

19. **Search nodes by label**
    - Function: `std::vector<int> searchNodes(const std::string& query);`
    - Variables: `query`
    - Constraints: Case-insensitive, partial match.

20. **Filter nodes by attribute**
    - Function: `std::vector<int> filterNodesByAttribute(const std::string& attr, const std::string& value);`
    - Variables: `attr`, `value`
    - Constraints: Attribute must exist.

## 5. Editing

21. **Add node**
    - Function: `int addNode(const std::string& label);`
    - Variables: `nodes`
    - Constraints: Unique label.

22. **Remove node**
    - Function: `bool removeNode(int nodeId);`
    - Variables: `nodes`, `edges`
    - Constraints: Remove connected edges.

23. **Add edge**
    - Function: `bool addEdge(int from, int to, double weight = 1.0);`
    - Variables: `edges`
    - Constraints: Nodes must exist, no duplicate edge.

24. **Remove edge**
    - Function: `bool removeEdge(int from, int to);`
    - Variables: `edges`
    - Constraints: Edge must exist.

25. **Edit node label**
    - Function: `bool editNodeLabel(int nodeId, const std::string& newLabel);`
    - Variables: `nodes`
    - Constraints: Unique label.

26. **Edit edge weight**
    - Function: `bool editEdgeWeight(int from, int to, double newWeight);`
    - Variables: `edges`
    - Constraints: Edge must exist.

## 6. Analytics

27. **Show node degree**
    - Function: `int getNodeDegree(int nodeId);`
    - Variables: `edges`
    - Constraints: Node must exist.

28. **Show average degree**
    - Function: `double computeAvgDegree();`
    - Variables: `nodes`, `edges`
    - Constraints: Non-empty graph.

29. **Count isolated nodes**
    - Function: `int countIsolatedNodes();`
    - Variables: `nodes`, `edges`
    - Constraints: N/A

30. **Show edge count**
    - Function: `int getEdgeCount();`
    - Variables: `edges`
    - Constraints: N/A

31. **Show graph density**
    - Function: `double computeDensity();`
    - Variables: `nodes`, `edges`
    - Constraints: N/A

32. **Highlight connected components**
    - Function: `std::vector<std::vector<int>> findConnectedComponents();`
    - Variables: `nodes`, `edges`
    - Constraints: N/A

33. **Visualize shortest path**
    - Function: `std::vector<int> shortestPath(int from, int to);`
    - Variables: `edges`
    - Constraints: Nodes must exist, handle no path.

34. **Show proximity depth**
    - Function: `int getProximityDepth(int nodeId);`
    - Variables: `edges`
    - Constraints: Node must exist.

## 7. Layouts & Views

35. **Support multiple layouts (e.g., Book View)**
    - Function: `void renderBookView(const Graph& graph);`
    - Variables: `nodes`, `edges`
    - Constraints: Layout algorithm.

36. **Switch between layouts**
    - Function: `void setLayout(LayoutType type);`
    - Variables: `enum LayoutType`
    - Constraints: Valid layout type.

37. **Display overlays (analytics, weights)**
    - Function: `void toggleOverlay(OverlayType type);`
    - Variables: `enum OverlayType`
    - Constraints: N/A

## 8. Console & UI

38. **Show viewer menu**
    - Function: `void drawViewerMenu();`
    - Constraints: Update with current key bindings.

39. **Show status bar**
    - Function: `void drawStatusBar();`
    - Variables: `currentMode`, `zoomLevel`, `focus info`
    - Constraints: Fit to console width.

40. **Display error messages**
    - Function: `void showError(const std::string& message);`
    - Constraints: Non-blocking.

41. **Display help/usage**
    - Function: `void showHelp();`
    - Constraints: List all commands.

## 9. Performance

42. **Efficient rendering for large graphs**
    - Function: `void renderGraph(const Graph& graph);`
    - Variables: `screenBuffer`
    - Constraints: Optimize for speed.

43. **Limit max nodes/edges**
    - Variable: `const int MAX_NODES; const int MAX_EDGES;`
    - Constraints: Prevent overflow.

44. **Incremental updates**
    - Function: `void updateGraphView();`
    - Constraints: Only redraw changed parts.


API
Logging
Unit Tests
Performance Benchmarking
Code Documentation
3D Rendering
Custom Analytics