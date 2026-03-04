# CBT Graph Viewer & Brain Modeling Suite

The CBT Graph Viewer is a high-performance console-based visualization and exploration tool for structured node-link data, now extended with a comprehensive Brain Modeling and Simulation suite. It features deterministic simulation kernels, hierarchical region mapping, and advanced interactive layouts.

## Key Features

- **Advanced Graph Rendering**
  - **Dynamic Zoom & Pan:** Smooth canvas navigation with adaptive node sizing and label wrapping.
  - **Layered Visibility:** Independent control over surface, node, edge, and overlay layers.
  - **Z-Buffering:** Ensures correct depth occlusion in complex topologies.

- **Brain Modeling Suite**
  - **Hierarchical Regions:** Support for multi-level brain region definitions (Lobe -> Region -> Sub-region).
  - **Simulation Kernel:** A perfectly deterministic simulation engine with snapshot and restore capabilities.
  - **Temporal Engine:** Playback and interpolation of time-series brain activity data.
  - **ROI Queries:** Spatial queries for nodes within specific Regions of Interest using an Octree-indexed spatial catalog.

- **Interactive Navigation**
  - **Standard View:** Traditional graph exploration.
  - **Book View:** Hierarchical subject-based organization (Hotkey: `B`).
  - **Cycle Layouts:** Switch between various visualization strategies (Hotkey: `N`).
  - **Search & Focus:** Keyword-based search and multi-node focus management.

- **Graph Analytics**
  Real-time computation of clustering coefficients, diameter, density, and betweenness centrality.

- **Data Operations**
  Robust CSV and JSON loading/saving. Multi-format export, including SVG snapshots.

## Project Structure

- `apps/viewer/`: Primary interactive application entry point.
- `src/`: Core library logic, including rendering, simulation, and graph theory components.
- `tests/`: Extensive testing suite:
  - `unit/`: Granular logic verification.
  - `bdd/`: Behavioral scenarios using Gherkin syntax (GIVEN/WHEN/THEN).
- `docs/`: Technical guides, API documentation, and research findings.

## Build and Installation

The project uses a standard `Makefile` for compilation.

```bash
# Build all targets (Viewer, Unit Tests, BDD Tests)
make all

# Clean build artifacts
make clean
```

## Running the Application

### Interactive Mode
```bash
./build/viewer
```

### Headless / CLI Mode
The viewer supports several automation flags:
```bash
./build/viewer --load-graph data.csv --get-node-details 42
```

## Testing

A comprehensive test suite is included to ensure system stability across updates.

```bash
# Run Unit Tests
./build/unit_tests

# Run BDD Scenarios
./build/bdd_tests
```

---

Maintained by Tamaro. This project is a foundational tool for brain-wide activity mapping and structural connectivity research.
