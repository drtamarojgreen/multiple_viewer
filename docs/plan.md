# Enhancement Implementation Plan

This document outlines the plan for implementing key enhancements for the CBT Graph Viewer application. The features covered are derived from the project requirements and issue tracking log.

---

## 1. External API for Automation (Req. 52)

**Objective:**
To provide an external API that allows programmatic control over the application for automation, scripting, and integration with other tools.

**Current Status:**
The application currently lacks any external API, forcing all interactions to be manual through the UI. This is tracked as issue #11.

**Proposed Solution:**
We will implement a command-line interface (CLI) API as a first step, as it aligns well with the console-based nature of the application and is faster to implement than a full REST API.

*   **Phase 1: Core CLI Framework**
    *   Implement a command-line argument parser (e.g., using a library like `cxxopts` or a simple custom one).
    *   Define a set of initial commands for core functionalities:
        *   `--load-graph <filepath>`: Load a graph from a CSV file.
        *   `--save-graph <filepath>`: Save the current graph to a CSV file.
        *   `--run-command <command>`: Execute an internal application command (e.g., 'pan-right', 'zoom-in').
        *   `--get-node-details <nodeId>`: Output details for a specific node.
    *   Modify the application's entry point (`main` function) to handle CLI arguments and execute them, potentially in a non-interactive "headless" mode.

*   **Phase 2: Expanded Command Set**
    *   Add commands for graph manipulation: `add-node`, `remove-node`, `add-edge`, `remove-edge`.
    *   Add commands for analytics: `get-degree <nodeId>`, `get-density`, `find-components`.
    *   Add commands for exporting data: `export-analytics <type> <filepath>`.

**Dependencies:**
*   None for Phase 1.
*   Phase 2 depends on the completion of the respective analytics and editing functions.

**Testing Strategy:**
*   **Unit Tests:** Test the argument parser and individual command handlers.
*   **Integration Tests:** Create shell scripts that run the application with various CLI arguments and validate the output (e.g., file content, stdout).

**Documentation:**
*   Create a new `docs/api_guide.md` detailing all available CLI commands, arguments, and examples.
*   Update the main `README.md` to mention the new CLI capabilities.

---

## 2. Logging Framework

**Objective:**
To implement a structured logging system for debugging, monitoring application health, and tracking user actions.

**Current Status:**
No formal logging system exists. Debugging relies on `printf` statements or a debugger.

**Proposed Solution:**
Integrate a lightweight, header-only C++ logging library like `spdlog` for its performance and flexibility.

*   **Phase 1: Setup and Basic Logging**
    *   Integrate `spdlog` into the project build system.
    *   Create a central `Logger` utility class to initialize and configure the logger (e.g., set log level, define log format).
    *   Configure two default sinks:
        *   A rotating file sink (`logs/app.log`) for persistent logging.
        *   A console sink for real-time output during development (configurable log level).
    *   Replace critical `std::cerr` or `std::cout` error messages with logger calls (`logger->error(...)`).

*   **Phase 2: Comprehensive Logging**
    *   Add informational logs for key events: application start/stop, graph loading/saving.
    *   Add debug-level logs for detailed flow tracing within complex functions (e.g., rendering, layout algorithms).
    *   Add trace-level logs for performance-critical sections.
    *   Allow the log level to be set via a configuration file or a CLI argument.

**Dependencies:**
*   None.

**Testing Strategy:**
*   **Unit Tests:** Test the `Logger` utility class to ensure correct initialization and configuration.
*   **Manual Verification:** Check log files and console output during testing to ensure messages are being logged correctly and at the appropriate levels.

**Documentation:**
*   Create a `docs/logging_guide.md` for developers, explaining how to use the logger, the different log levels, and the log file location and format.

---

## 3. Unit & Integration Testing (Req. 57, 58)

**Objective:**
To establish a robust testing framework and increase test coverage to ensure code quality, prevent regressions, and facilitate safer refactoring.

**Current Status:**
Unit and integration test coverage is incomplete or missing entirely, as noted in issues #12 and #13.

**Proposed Solution:**
We will adopt the Google Test framework for both unit and integration testing.

*   **Phase 1: Framework Setup and Core Logic Tests**
    *   Integrate Google Test into the build system.
    *   Create a separate `tests` directory.
    *   Prioritize writing unit tests for critical, non-UI logic:
        *   `Graph` class methods (add/remove nodes/edges, validation).
        *   Analytics functions (`getNodeDegree`, `computeDensity`, etc.).
        *   CSV parsing and serialization logic.

*   **Phase 2: Expanding Test Coverage**
    *   Write unit tests for user interaction logic (e.g., `panView`, `zoomIn`, `cycleFocus`), mocking any UI dependencies.
    *   Develop integration tests that simulate user workflows:
        *   Load a graph -> perform edits -> run analytics -> save the graph -> verify the final file.
        *   Test edge cases like loading malformed files or operating on an empty graph.

*   **Phase 3: CI Integration**
    *   Set up a Continuous Integration (CI) pipeline (e.g., using GitHub Actions) to automatically build and run all tests on every push/pull request.

**Dependencies:**
*   None.

**Testing Strategy:**
*   This task *is* the testing strategy. The goal is to achieve a target code coverage (e.g., 80%) for all non-rendering code.

**Documentation:**
*   Create a `tests/README.md` explaining how to build and run the tests locally.
*   Document the CI setup and process.

---

## 4. Performance Benchmarking (Req. 59)

**Objective:**
To measure, track, and optimize the performance of critical application functions, especially for large graphs.

**Current Status:**
Performance is unmeasured, and potential bottlenecks are unknown, as tracked in issue #14.

**Proposed Solution:**
We will use the Google Benchmark library to create a dedicated benchmarking suite.

*   **Phase 1: Setup and Core Benchmarks**
    *   Integrate Google Benchmark into the build system.
    *   Create a `benchmarks` directory.
    *   Implement benchmarks for:
        *   `loadGraphFromCSV` with varying graph sizes (small, medium, large).
        *   Core analytics functions (`findConnectedComponents`, `shortestPath`).

*   **Phase 2: Rendering and Interaction Benchmarks**
    *   Benchmark the main `renderGraph` function. This may require abstracting the rendering logic from the actual screen drawing to measure the buffer preparation time.
    *   Benchmark graph manipulation operations (`addNode`, `removeNode`) on large graphs.

*   **Phase 3: Profiling and Optimization**
    *   Use the benchmark results to identify slow code paths.
    *   Use a profiler (e.g., gprof, Valgrind/Callgrind, Visual Studio Profiler) to analyze the identified bottlenecks.
    *   Apply optimizations based on profiling data (e.g., improving data structures, optimizing algorithms).

**Dependencies:**
*   None.

**Testing Strategy:**
*   The benchmarks themselves are a form of testing. Results should be consistent and reproducible. The CI pipeline should optionally be able to run benchmarks to detect performance regressions.

**Documentation:**
*   Create a `benchmarks/README.md` explaining how to build and run the benchmarks.
*   Maintain a `docs/performance.md` file to log benchmark results over time and document significant optimizations.

---

## 5. Code Documentation (Req. 60)

**Objective:**
To create comprehensive and automatically generated documentation for the codebase, making it easier for current and future developers to understand and contribute.

**Current Status:**
Code documentation is sparse and inconsistent, as noted in issue #15.

**Proposed Solution:**
We will adopt Doxygen as the documentation generator and enforce a consistent comment style.

*   **Phase 1: Setup and Initial Pass**
    *   Create a Doxygen configuration file (`Doxyfile`) in the project root.
    *   Add Doxygen-style docstrings (`/** ... */`) to all public classes, methods, and functions in the core `Graph` and `Node`/`Edge` structures.
    *   Generate the initial HTML documentation and add it to `.gitignore`.

*   **Phase 2: Full Coverage**
    *   Go through the entire codebase and add docstrings to all remaining functions and files.
    *   Explain the purpose of each function, its parameters, and what it returns.
    *   Integrate the Doxygen generation step into the CI pipeline to ensure documentation stays up-to-date.

**Dependencies:**
*   None.

**Documentation:**
*   This task is about creating documentation. The output will be a browsable HTML documentation set.
*   Add a section to the main `README.md` on how to generate the documentation locally.

---

## 6. 3D Graph Rendering (Req. 71)

**Objective:**
To provide an alternative 3D rendering mode for visualizing complex graph structures. This is considered an advanced, optional feature.

**Current Status:**
The application is strictly 2D, as noted in issue #26.

**Proposed Solution:**
Given the console-based nature, a true 3D rendering is complex. We will implement a "2.5D" ASCII projection as a first step.

*   **Phase 1: 3D Data Structures**
    *   Extend the `Node` struct to include a `z` coordinate: `struct Node { int id; std::string label; int x, y, z; };`.
    *   Update loading/saving functions to handle the new coordinate (optional, can be auto-generated).

*   **Phase 2: 3D Layout Algorithm**
    *   Implement a simple 3D layout algorithm. A force-directed layout extended to 3D is a good candidate.
    *   Initially, this can be a simple random distribution of Z-coordinates.

*   **Phase 3: 2.5D ASCII Projection Renderer**
    *   Implement a projection function that converts 3D coordinates (`x, y, z`) to 2D screen coordinates (`screen_x`, `screen_y`). A simple orthographic or perspective projection can be used.
    *   The `z` coordinate can influence the character used for the node (e.g., smaller/dimmer characters for nodes further away) and its draw order (nodes with higher `z` are drawn on top).
    *   Update the `renderGraph` function to use the new 3D renderer when in "3D mode".

**Dependencies:**
*   Requires a layout switching mechanism (Req. 36) to toggle between 2D and 3D views.

**Testing Strategy:**
*   **Unit Tests:** Test the 3D projection math and layout algorithm logic.
*   **Visual/Manual Tests:** Manually inspect the rendered output to ensure the 3D effect is coherent and nodes are rendered correctly.

**Documentation:**
*   Update user documentation to explain the 3D mode, how to activate it, and its limitations.
*   Document the 3D layout and rendering algorithms for developers.

---

## 7. Custom Analytics Scripts (Req. 50, 51, 90)

**Objective:**
To allow users to extend the application's functionality by writing and running their own analytics scripts.

**Current Status:**
All analytics are hardcoded. There is no plugin or scripting system, as noted in issues #9, #10, and #45.

**Proposed Solution:**
Integrate an embedded scripting language. Lua is an excellent choice due to its small footprint, simple C API, and ease of sandboxing.

*   **Phase 1: Scripting Engine Integration**
    *   Integrate the Lua scripting engine into the application.
    *   Create a C++ "binding" layer to expose core graph data and functions to the Lua environment.
    *   Expose read-only functions first:
        *   `graph.getNodeCount()`
        *   `graph.getEdgeCount()`
        *   `graph.getNodes()` (returns a table of node IDs)
        *   `graph.getNode(id)` (returns a table with node data: label, x, y)
        *   `graph.getEdges()`

*   **Phase 2: Running Scripts**
    *   Implement a mechanism to load and execute a Lua script file (e.g., via a CLI command `--run-script <script.lua>`).
    *   The script's output (from `print()` calls in Lua) will be directed to the console.

*   **Phase 3: Advanced Bindings and Sandboxing**
    *   Expose graph modification functions (`graph.addNode(...)`, etc.) to the scripting environment.
    *   Implement sandboxing to prevent scripts from accessing the file system or other sensitive OS functions, ensuring they can only interact with the graph data provided.
    *   Allow scripts to return structured data (e.g., a list of node IDs to highlight) that the application can then use for visualization.

**Dependencies:**
*   The CLI API (Req. 52) would be useful for triggering scripts.

**Testing Strategy:**
*   **Unit Tests:** Test the C++/Lua binding functions individually.
*   **Integration Tests:** Create a suite of test Lua scripts (both valid and invalid) and run them through the application, asserting the correctness of the output or the state of the graph.

**Documentation:**
*   Create a `docs/scripting_api.md` that documents the entire Lua API exposed by the application, with examples for each function.
*   Provide a
