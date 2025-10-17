# CBT Graph Viewer - Comprehensive Enhancement Roadmap

This document outlines a long-term vision for the CBT Graph Viewer, containing over 100 potential enhancements. The features are designed to be implemented without new external library dependencies and are categorized to provide a clear, professional roadmap.

---

### **I. UI/UX & Interactivity**
*Enhancements focused on improving user experience, control, and overall usability.*

1.  **Command Palette:** Implement a `:`-style command mode for direct command execution (e.g., `focus <id>`, `save <file>`).
2.  **Focus History:** Track focus changes and allow navigation back/forward through the history.
3.  **Node Bookmarking:** Allow users to save and cycle through bookmarked nodes or node sets.
4.  **Multi-Select Mode:** Enable selection of multiple nodes for batch operations (e.g., focus, delete, export).
5.  **Context-Sensitive Help:** Implement a `?` key that displays help relevant to the current view or mode.
6.  **Customizable Keybindings:** Allow users to define their own keybindings via a configuration file.
7.  **"Zen Mode":** A toggle to hide all UI elements, leaving only the graph visualization.
8.  **Status Bar:** A persistent bar showing the current file, focus ID, zoom level, and pan coordinates.
9.  **Notification System:** A standardized, non-blocking way to show feedback (e.g., "File saved.").
10. **Interactive Tutorial:** A first-launch guide that walks users through the basic features.
11. **Color Theme Support:** Allow users to define UI and graph colors in a configuration file.
12. **Search History:** Let users access and re-run previous search queries.
13. **Regex-Based Search:** Enhance search to support regular expressions for advanced queries.
14. **Smooth Panning/Zooming:** Animate transitions between pan and zoom states for a smoother feel.
15. **"Drill Down" Mode:** A mode to temporarily isolate a node and its immediate neighbors.
16. **Quick-Find by ID:** A dedicated prompt to jump directly to a node by its index.
17. **Mouse-Click to Focus:** (Platform-specific) Use console APIs to allow focusing nodes with a mouse click.
18. **Mouse-Hover Info:** (Platform-specific) Show a node's label in the status bar on mouse hover.
19. **Mouse-Drag to Pan:** (Platform-specific) Allow panning the canvas by clicking and dragging.
20. **Scroll Wheel to Zoom:** (Platform-specific) Use the mouse scroll wheel for zooming in and out.

---

### **II. Reporting & Data Export**
*Features for generating reports, summaries, and exporting graph data into various formats.*

21. **Export Analytics Panel:** Save the full analytics summary to a text or markdown file.
22. **Export Node "Page View":** Export the detailed view of a single node to a shareable markdown file.
23. **Export "Book View":** Export the chapter-based "Book View" to a structured markdown document.
24. **Export ASCII Snapshot:** Save the current ASCII rendering of the graph to a text file.
25. **Export to GraphML:** Implement an exporter for the standard GraphML format for use in other tools.
26. **Export to GEXF:** Add an exporter for the GEXF format, common in network analysis software.
27. **Export to JSON:** Implement a JSON exporter (e.g., node-link format).
28. **Export Filtered Graph:** Allow exporting only the currently visible or filtered set of nodes and edges.
29. **Generate Connectivity Report:** A report listing all connected components, their sizes, and member nodes.
30. **Generate Degree Distribution Report:** A text report showing the distribution of node degrees (e.g., how many nodes have 1, 2, 3... neighbors).
31. **Generate Search Results Report:** Export the list of nodes matching a search query to a file.
32. **Export Shortest Path Report:** After finding a path, allow exporting the nodes and path length to a file.
33. **Export Subgraph Analytics:** Generate an analytics report for a specific, isolated subgraph or component.
34. **Batch Export Node Pages:** Export all "Page Views" for nodes of a given subject into a single directory.
35. **Customizable Report Templates:** Allow users to define simple templates for how reports are formatted.
36. **Include Metadata in Reports:** Add options to include timestamps, source filenames, and graph summary stats in report headers.
37. **Export Adjacency List:** Export the graph as a simple text-based adjacency list.
38. **Export Adjacency Matrix:** Export the graph as a CSV representing the adjacency matrix.
39. **Generate "Diff" Report:** A tool to compare two graph files and report on added/removed nodes and edges.
40. **Scriptable Reporting:** Allow chaining report commands from a file for batch processing.

---

### **III. Advanced Analytics & Insights**
*Deeper analytical capabilities to uncover graph properties and insights.*

41. **Betweenness Centrality:** (On-demand) Calculate for a selected node to measure its importance as a "bridge".
42. **Closeness Centrality:** (On-demand) Calculate for a selected node to measure its "centrality" in terms of reachability.
43. **Eigenvector Centrality:** (On-demand) Calculate to identify influential nodes connected to other influential nodes.
44. **PageRank Algorithm:** (On-demand) Implement PageRank to measure node importance.
45. **Cycle Detection:** A tool to detect and report cycles within the graph.
46. **Bridge Detection:** An analysis to find edges whose removal would increase the number of connected components.
47. **Community Detection:** (Simplified) Implement an algorithm like Girvan-Newman to identify communities.
48. **Keyword Co-occurrence Analysis:** Find nodes frequently connected to nodes matching a keyword.
49. **Graph Density Trend:** Analyze density changes across a series of loaded files.
50. **"What-If" Analysis:** A mode to preview the analytics impact of removing a node or edge.
51. **Node Similarity Score:** Calculate a similarity score between two nodes based on shared neighbors (Jaccard similarity).
52. **Global Clustering Coefficient:** Calculate the overall clustering coefficient for the entire graph.
53. **Average Path Length:** Calculate the average shortest path length between all pairs of nodes.
54. **Graph Diameter:** Find the longest shortest path in the graph.
55. **Articulation Point Identification:** Find nodes (cut vertices) whose removal would split a component.
56. **Subgraph Isomorphism Search:** (Advanced) A tool to find specific patterns (subgraphs) within the main graph.
57. **Structural Hole Detection:** Identify nodes that bridge otherwise disconnected parts of the network.
58. **Assortativity Calculation:** Measure the tendency for nodes to connect to other nodes of similar degree.
59. **Reciprocity Calculation:** (For directed graphs) Measure the proportion of mutual connections.
60. **K-Core Decomposition:** Analyze the graph's core-periphery structure.

---

### **IV. Visualization & Rendering**
*Improvements to the visual representation of the graph.*

61. **Custom Node Glyphs:** Allow the node's character to be defined by an attribute in the data.
62. **Edge Weight Visualization:** Render edges with different characters (`.`, `-`, `=`) based on weight.
63. **Alternative Layout Algorithms:** Add options for Circular, Hierarchical, and Grid layouts.
64. **Color-Coding:** Use ANSI colors to style nodes based on subject, community, or centrality score.
65. **Path Highlighting:** Use color or different glyphs to highlight the path between two nodes.
66. **"Ghost" Mode:** Show filtered-out nodes dimly in the background instead of hiding them completely.
67. **Render Labels Inside Nodes:** At high zoom levels, draw the label inside the node block.
68. **ASCII Anti-Aliasing:** Use a character ramp (`.`, `:`, `o`, `O`, `@`) to create smoother-looking lines.
69. **Perspective Label Scaling:** Make labels smaller as they get further from the focus point.
70. **Fisheye Lens View:** A rendering mode that magnifies the area around the focus point.
71. **Adjustable Node Spacing:** Allow the user to control the padding between nodes in the layout.
72. **Toggle Edge/Label Visibility:** Hotkeys to quickly hide or show all edges or all labels.
73. **Directed Edge Arrowheads:** Add `>` characters to the end of lines to indicate edge direction.
74. **Curved Line Rendering:** (Advanced) Implement an algorithm to draw curved edges to reduce line overlap.
75. **Thematic Presets:** Pre-defined sets of rendering options for different tasks (e.g., "Analytics", "Editing").
76. **Render Node Weight:** Display the node's weight value next to its label.
77. **Highlight Isolated Nodes:** Render nodes with zero degree in a distinct color or with a unique glyph.
78. **Highlight Bridges:** Render bridge edges in a distinct color.
79. **Dynamic Grid Overlay:** A toggleable background grid to help judge distance and alignment.
80. **Viewport Clipping Indicator:** Show an indicator on nodes/edges that are partially off-screen.

---

### **V. Data Management & Persistence**
*Enhancements related to loading, saving, and editing graph data.*

81. **In-Viewer Node Editing:** An interactive prompt to edit a selected node's label or weight.
82. **Interactive Edge Editing:** Prompts to add or remove an edge between two specified nodes.
83. **"Undo" Stack:** A multi-level undo system for all editing operations.
84. **Auto-Save Functionality:** Automatically save changes to a backup file at regular intervals.
85. **Session Management:** Save and load the complete application state (pan, zoom, focus, filters).
86. **Graph Merging:** A tool to merge a loaded graph with another graph file.
87. **Flexible CSV Parsing:** Add options to handle CSV files with or without headers.
88. **Create New Graph:** A `File > New` command to start a new graph from scratch in the editor.
89. **Data Validation on Load:** A comprehensive check for data integrity (e.g., duplicate IDs, broken edges) on file load.
90. **Error Recovery:** Better handling of malformed lines in CSV files (skip and report instead of failing).
91. **Transactional Edits:** A mode to group several edits together and then "commit" them at once.
92. **Read-Only Mode:** A toggle to prevent accidental edits to the graph data.
93. **Support for Comments in Data Files:** Ignore lines in CSV files that start with a `#` character.
94. **Automatic Backups:** Create a `.bak` file when overwriting an existing graph file.
95. **Partial Graph Loading:** An option to load only the first N nodes or nodes matching a filter for large files.

---

### **VI. Core Engine & Performance**
*Architectural improvements for speed, efficiency, and scalability.*

96. **Incremental Rendering:** Optimize the render loop to only redraw parts of the screen that have changed.
97. **Analytics Caching:** Cache the results of expensive, on-demand analytics calculations.
98. **Background Threading:** (Advanced) Move expensive calculations to a background thread to keep the UI responsive.
99. **Optimized Graph Structure:** Switch from `std::vector` + `std::map` to a more efficient adjacency list representation.
100. **Memory Profiling:** Add internal metrics to track memory usage for large graphs.
101. **Spatial Indexing:** Implement a quad-tree or similar structure for faster collision detection and rendering.
102. **Parallelize Layouts:** Use multiple threads to speed up force-directed layout calculations.
103. **Incremental Layouts:** Update the layout algorithm to adjust positions incrementally instead of recalculating from scratch.
104. **Efficient Z-Buffer:** Optimize the Z-buffer implementation for faster depth checks.
105. **Performance Profiling Hooks:** Add internal timers to measure the performance of key functions (rendering, analytics).