# Graph Data Store Design

This document outlines the design of the data structure used to store and manage graph data in the CBT Graph Viewer application.

## Current Data Structure

The current graph data structure is defined in `map_logic.h` and consists of two main components: the `GraphNode` struct and the `Graph` class.

### `GraphNode` Struct

The `GraphNode` struct represents a single node in the graph. It contains the following fields:

- **`label`**: A `std::string` that holds the name or label of the node. This is displayed in the viewer.
- **`index`**: An `int` that serves as a unique identifier for the node.
- **`neighbors`**: A `std::vector<int>` that stores the indices of the nodes connected to this node. This forms the adjacency list for the node.
- **`weight`**: An `int` that represents the weight or importance of the node.
- **`subjectIndex`**: An `int` used to group nodes by subject or category.

### `Graph` Class

The `Graph` class is the main container for the entire graph. It manages all the nodes and their relationships. The key fields for data storage are:

- **`nodes`**: A `std::vector<GraphNode>` that stores all the nodes in the graph. This provides fast, indexed access to the nodes.
- **`nodeMap`**: An `std::unordered_map<int, GraphNode>` that maps a node's index to the `GraphNode` object. This allows for efficient lookups of nodes by their index.

This hybrid approach of using both a `std::vector` and an `std::unordered_map` provides the benefits of both fast indexed access and quick key-based lookups, which is effective for the application's needs.

## Proposed Improvements

While the current data structure is well-suited for the application's needs, the following enhancements could provide greater flexibility and support for more advanced features.

### Adjacency List for Sparse Graphs

The current use of `std::vector<int> neighbors` is an effective implementation of an adjacency list. This is an excellent choice for the type of sparse graphs that are typically visualized in this application, as it efficiently stores connections without the overhead of an adjacency matrix.

### Support for Directed Edges

The current implementation assumes undirected edges. To support directed graphs, the `GraphNode` struct could be modified to distinguish between incoming and outgoing edges:

```cpp
struct GraphNode {
    // ... existing fields
    std::vector<int> out_neighbors; // Nodes this node points to
    std::vector<int> in_neighbors;  // Nodes pointing to this node
};
```

This would allow for more complex graph analyses, such as dependency tracking or flow visualization.

### Edge Properties

To represent more complex relationships, an `Edge` struct could be introduced to store properties such as weight, relationship type, or labels.

```cpp
struct Edge {
    int targetNodeIndex;
    int weight;
    std::string label;
};

struct GraphNode {
    // ... existing fields
    std::vector<Edge> neighbors;
};
```

This would enable the visualization of richer graph data, where the connections themselves contain important information.
