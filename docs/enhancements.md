# CBT Graph Viewer - Future Enhancements

This document outlines potential future enhancements for the CBT Graph Viewer, focusing on **Reporting, Analytics, and Data Portalization**. The goal is to expand the application's functionality without introducing any new external library dependencies.

## 1. Reporting

- **Export Analytics Summary:**
  - Implement a feature to export the full analytics panel (`[G]`) to a text file (e.g., `analytics_report.txt`). This would include node/edge counts, connectivity status, average degree, and lists of top/least connected nodes.
- **"Page View" to File:**
  - Allow the user to export the detailed "Page View" (`[E]`) for a specific node to a markdown file. This would create a simple, shareable report for individual nodes of interest.
- **Book View to Markdown:**
  - Enhance the "Book View" (`[B]`) to be exportable as a structured markdown document, with each chapter as a heading and the nodes listed underneath. This would provide a simple way to generate a table of contents for the graph's subjects.
- **Save Snapshot to ASCII:**
  - Implement a feature to save the current visual representation of the graph (the ASCII layout) to a text file. This would allow users to capture and share a specific view of the graph at a moment in time.

## 2. Analytics

- **Centrality Measures:**
  - **Betweenness Centrality (On-Demand):** Implement a command to calculate the betweenness centrality for a selected node. This would measure how often a node lies on the shortest path between other nodes, indicating its importance as a "bridge" in the graph.
  - **Closeness Centrality (On-Demand):** Add a command to calculate the closeness centrality for a selected node, which measures its average distance to all other nodes. This would identify nodes that are "central" in terms of reachability.
- **Component Analysis:**
  - In addition to counting connected components, add a feature to list the nodes belonging to each component.
  - Implement a command to cycle focus only through the nodes of the largest or a selected component.
- **Pathfinding:**
  - Add a feature to find and highlight the shortest path between two user-selected nodes. The path could be displayed in the analytics panel or by changing the glyphs of the nodes/edges in the path.
- **Keyword Co-occurrence:**
  - Implement an analysis tool that finds nodes that are frequently connected to nodes matching a specific keyword. For example, finding all nodes that are neighbors of nodes with the label "Therapy".

## 3. Data Portalization

- **Interactive Search and Filter:**
  - Enhance the search functionality (`/`) to be more interactive. Instead of just focusing the results, it could temporarily filter the view to show *only* the matching nodes and their direct neighbors.
- **"Drill-Down" Mode:**
  - Create a mode where focusing on a node and pressing a key (e.g., `Enter`) "collapses" the view to only show that node and its neighbors. Pressing the key again would "zoom back out" to the previous view. This would create a more focused exploration experience.
- **Subject-Based Filtering:**
  - Allow the user to select a "Subject" and filter the graph to only show nodes belonging to that subject. This would be a more powerful version of the existing `subjectFilterOnly` flag.
- **Saved Views/Sessions:**
  - Implement a mechanism to save the current state of the viewer—including pan/zoom, focused nodes, and filter settings—to a session file. This would allow users to resume their exploration later or share a specific "view" of the data with others.