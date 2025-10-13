# CBT Graph Viewer

The CBT Graph Viewer is a console-based graph visualization and exploration tool tailored for structured node-link data. It features zoomable rendering, analytic overlays, and advanced node labeling with support for focus cycling and wrapped text.

## Features

- **Graph Rendering**
  - Displays nodes as square blocks (size based on zoom level)
  - Labels are wrapped and aligned beneath each node
  - Edge connections rendered with Bresenham-style ASCII lines
  - Z-buffering ensures proper depth visibility

- **Interactive Navigation**
  - Arrow keys or IJKL for panning the canvas
  - `Z` / `X` to zoom in and out
  - `TAB` cycles through focused nodes
  - `/` enables label-based keyword search
  - `F` / `O` to add/remove focused nodes
  - `T` to set max render depth

- **Graph Analytics**
  - `[P]` toggles full analytics overlay:
    - Node/edge counts
    - Connectivity status
    - Average degree and isolation stats
    - Topic weights and top/least connected nodes
    - Render and load performance metrics

- **Book and Page Views**
  - `[B]` presents chapter-style breakdown grouped by subject and depth
  - `[E]` displays full metadata and neighbors for a specific node

- **Data Operations**
  - `[S]` saves graph as CSV
  - `[L]` loads graph from CSV
  - Adds new nodes with `[A]` and removes with `[R]`

## Display Configuration

- Optimized for PowerShell or external terminals supporting `system("cls")`
- 80x25 console dimensions assumed by default
- Supports ANSI-based soft clearing for alternate terminals

## Development Status

- Label wrapping, zoom sizing, and rendering logic fully implemented
- Search, analytics panel, and view modes now interactive and functional
- Panning logic corrected to properly shift the graph relative to viewport
- Menu reflects current hotkey mappings and available commands

## Planned Improvements

- Optional mouse or pointer navigation
- Export snapshot to file with layout preview
- Custom glyph styling for weighted or highlighted nodes

## Build and Test

To build the application, run the following command:

```bash
g++ -std=c++17 *.cpp -o main
```

To run the built-in test suite, execute the compiled application with the `--test` flag:

```bash
./main --test
```

## Usage

Compile and run the application in a terminal. Use interactive hotkeys to explore graph data, manage focus, and reveal structural analytics.

---

Maintained by Tamaro. For feedback, improvements, or enhancement ideas, reach out directly or submit patches via your development workflow.
