# CBT Graph Viewer - Command-Line API Guide

This document provides a guide to using the command-line interface (CLI) for the CBT Graph Viewer. The CLI allows for headless operation, scripting, and automation of graph tasks.

---

## 1. Basic Usage

The application can be run in CLI mode by providing one or more command-line arguments. If no arguments are provided, the application will launch in its default interactive mode.

```bash
# Run in interactive mode
./graph_viewer

# Run in CLI mode with commands
./graph_viewer --load-graph graph.csv --get-node-details 1
```

---

## 2. Commands

### `--load-graph <filepath>`

**Purpose:** Loads a graph from a specified CSV file. This is typically the first command you will use, as most other commands operate on a loaded graph.

**Arguments:**
- `<filepath>`: The path to the input CSV file.

**Example:**
```bash
./graph_viewer --load-graph data/my_graph.csv
```

---

### `--save-graph <filepath>`

**Purpose:** Saves the current state of the graph to a specified CSV file. This is useful after performing modifications or running analyses.

**Arguments:**
- `<filepath>`: The path to the output CSV file. If the file already exists, it will be overwritten.

**Example:**
```bash
./graph_viewer --load-graph input.csv --save-graph output.csv
```

---

### `--get-node-details <nodeId>`

**Purpose:** Prints the details of a specific node to the console (stdout). This command requires a graph to be loaded first.

**Arguments:**
- `<nodeId>`: The integer ID of the node to inspect.

**Example:**
```bash
# Load a graph and then get details for node with ID 5
./graph_viewer --load-graph input.csv --get-node-details 5
```
