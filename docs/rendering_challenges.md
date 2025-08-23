# Rendering Challenges in a Console Environment

This document outlines some of the key challenges faced and addressed while improving the graph rendering capabilities of the CBT Graph Viewer. The primary goal was to display a larger number of nodes in a limited console space while maintaining readability.

---

## 1. Node Occlusion

**Challenge:**
The initial rendering logic placed nodes based on a simple Breadth-First Search (BFS) expansion from a focus point. The collision detection was minimal, only checking if the *center point* of a new node was already occupied. This led to significant visual overlap, where larger nodes would completely obscure smaller ones, making the graph unreadable.

**Solution:**
A more robust **Axis-Aligned Bounding Box (AABB)** collision detection system was implemented in the layout algorithm within `viewer_logic.cpp`.

-   Before placing a new node, the algorithm now calculates its future bounding box based on its calculated size.
-   It then checks this bounding box against the bounding boxes of all previously placed nodes.
-   A new node is only placed if its bounding box does not intersect with any other node's box.
-   This was achieved by checking if the distance between the centers of two nodes is less than half the sum of their sizes on both the X and Y axes (`abs(x1 - x2) * 2 < size1 + size2`).

This change fundamentally prevents nodes from being drawn on top of each other, resolving the most critical occlusion issues.

---

## 2. Visual Density and Scalability

**Challenge:**
Displaying a dense graph in a fixed-size console (e.g., 80x25 characters) is inherently difficult. The original implementation gave all nodes a uniform size based on a global zoom level. This was inefficient, as it treated a distant, less important node the same as a central, focused node. To show more nodes, a more intelligent approach to sizing was needed.

**Solution:**
Two key changes were made to improve visual density:

1.  **Depth-Based Node Sizing:** The `Graph::calculateNodeSize` function was modified to make node size dependent on its "depth" (its `z` coordinate, or distance from a focused node). Nodes farther away from the center of focus are now rendered smaller. This creates a natural perspective effect and allows more nodes to fit into the view without feeling cluttered. The size is calculated as `max(1, base_size - depth)`, where `base_size` is determined by the zoom level.

2.  **Adaptive Node Spacing:** The distance (`step`) at which a child node is placed from its parent was made adaptive. Instead of being a fixed value or based only on the child's size, the spacing is now calculated using the sizes of *both* the parent and the child node: `step = (parent_size / 2) + (child_size / 2) + padding`. This ensures that nodes are packed together as tightly as possible without overlapping, leading to a more compact and space-efficient layout.

---

## 3. Label Management

**Challenge:**
While the node bodies are now properly spaced, their text labels can still overlap, especially in dense areas of the graph. Labels are drawn starting from the bottom-left of a node's bounding box and can easily run into other nodes or other labels.

**Current Status & Future Work:**
This challenge has been noted but not fully resolved in the current implementation. The primary focus was on fixing the physical node layout. Solving label overlap is a complex problem in its own right and represents a key area for future improvement.

Potential solutions for the future include:
-   **Dynamic Label Placement:** Intelligently moving labels to different positions around a node (e.g., top, right, center) to avoid collisions.
-   **Label Truncation:** Automatically shortening long labels with "..." when they are in crowded areas.
-   **Prioritized Labels:** Only showing labels for focused nodes or nodes above a certain size/importance threshold, hiding others until the user zooms in.

These documented challenges and solutions provide a clear path for ongoing development of the graph viewer's rendering capabilities.

---

## 4. Alternative Layouts: The Nexus Flow Mode

**Challenge:**
The default perspective layout, based on a rigid BFS expansion, can sometimes create unnatural or confusing layouts, especially for highly interconnected or non-hierarchical graphs. It also provides a static view, which may not be ideal for understanding the dynamic relationships between nodes.

**Solution:**
A new experimental view mode, **Nexus Flow**, was introduced to provide a more organic and dynamic visualization of the graph. It can be activated by pressing the 'N' key in the interactive viewer.

-   **Force-Directed Layout:** Unlike the BFS-based layout, Nexus Flow uses a physics-based simulation to position the nodes. The algorithm treats nodes as physical objects that repel each other, while the edges between them act like springs that pull them together. This results in a layout where related nodes naturally cluster together, and the overall structure of the graph becomes more apparent.

-   **Dynamic "Flow":** The layout is not static. Each time the graph is rendered, the simulation runs for a few iterations, causing the nodes to shift and "flow" into a stable position. This creates a sense of a living, dynamic graph.

-   **Focus Highlighting:** Focused nodes are highlighted with a different glyph ('O') in this mode, making them easy to spot within the flowing layout.

This new mode provides an alternative way to explore the graph's topology, and is particularly useful for understanding complex relationships that are not well-represented by a simple tree-like structure.
