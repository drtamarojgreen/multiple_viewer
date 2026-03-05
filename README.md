# Multiple Viewer (CBT Graph Viewer + Brain Modeling Sandbox)

Multiple Viewer is a C++17 console application for exploring graph data and experimenting with brain-model overlays. It combines interactive navigation, graph analytics, headless CLI workflows, and a deterministic model/simulation core used by domain plugins and BDD scenarios.

## What it does

- Interactive terminal graph viewer/editor with pan/zoom, focus tools, search, multiple layout modes, and save/load flows.
- Headless CLI operations for loading graphs, saving graphs, and printing node metadata.
- Brain-model data loading for atlas, labels, and overlay mappings.
- Extended architecture modules for analytics, scripting runtime, IO management, layout, and UI helpers.
- Test coverage across unit-style logic checks and BDD feature scenarios (core kernel, UI, data, rendering, domain behavior).

## Repository layout

- `apps/viewer/` — main application entrypoint (`viewer`).
- `src/` — core graph/viewer logic and extension modules.
  - `src/model/` — brain model entities, repository, deterministic kernel/event bus, and domain plugins.
  - `src/render/`, `src/input/`, `src/layout/`, `src/analytics/`, `src/io/`, `src/scripting/`, `src/ui/`.
- `tests/` — unit-style and BDD test harnesses + step definitions.
- `docs/` — guides, architecture notes, and analysis documents.

## Build instructions (current)

This repository is built via the top-level `Makefile`.

### Prerequisites

- `g++` with C++17 support
- `make`

### Build everything

```bash
make all
```

This generates:

- `build/viewer`
- `build/unit_tests`
- `build/bdd_tests`

### Clean artifacts

```bash
make clean
```

### Build and run tests in one command

```bash
make test
```

## Running the viewer

### Interactive mode

```bash
./build/viewer
```

### Interactive mode with initial graph/model inputs

```bash
./build/viewer \
  --load-graph graph_input.csv \
  --load-atlas atlas.brn \
  --load-labels labels.txt \
  --load-overlay overlay.txt
```

### Show CLI help

```bash
./build/viewer --help
```

## Headless CLI workflows

Load a graph and print details for one node:

```bash
./build/viewer --load-graph graph_input.csv --get-node-details 42
```

Load and re-save a graph:

```bash
./build/viewer --load-graph graph_input.csv --save-graph out.csv
```

## Testing

After building (`make all`) you can run test binaries directly:

```bash
./build/unit_tests
./build/bdd_tests
```

Or run both through the Make target:

```bash
make test
```

## Notes

- A legacy `CMakeLists.txt` exists, but the actively maintained build flow in this repository is the `Makefile` targets above.
- The viewer is terminal-first; no GUI/web frontend is required for core usage.
