# UIPrinter Framework Guide

The `UIPrinter` is a mock rendering framework designed for automated behavioral testing of the CBT Graph Viewer's visual components. It implements the `render::IRenderer` interface, allowing it to be swapped into any context where a standard renderer would normally be used.

## 1. Core Concept

Unlike traditional renderers that output to a screen or console buffer, the `UIPrinter` captures all draw calls and system state into a `std::stringstream`. This text-based "frame" can then be searched or asserted against in BDD (Behavior-Driven Development) tests to verify that the UI is logically correct.

## 2. Captured State

The `UIPrinter` captures several key aspects of the viewer state in every frame:

- **Viewport Metadata:** Console dimensions, current pan offsets (`panX`, `panY`), zoom level, and active `ViewMode`.
- **Analytics Panel:** If enabled, the printer captures the current node and edge counts being displayed to the user.
- **Focus Set:** A list of all node indices currently in the viewer's focus set.
- **Node Rendering:** For every node in the graph, the printer logs:
    - Node ID and Label.
    - Calculated `ScreenCoord` (logical layout position + pan offsets).
    - Resolved Color (based on weight or other visual mappers).
    - **Visibility:** Whether the node is logically within the viewport bounds.

## 3. Usage in BDD

The `UIPrinter` is primarily used via the `ui_steps.cpp` definitions.

### Example Scenario:
```gherkin
Scenario: Viewport Culling
  Given a graph with node 100 at extreme coordinates
  And a UIPrinter is initialized
  When I render the graph to the printer
  Then node 100 should be marked as "Visible: NO" in the output
```

## 4. Extending the Printer

To capture new visual features (e.g., edge rendering, heatmap overlays), update the `UIPrinter::render` method in `tests/print/UIPrinter.cpp` to append relevant metadata to the `outputBuffer_`.
