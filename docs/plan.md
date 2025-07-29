# Enhancement Implementation Plan

This document outlines the plan for implementing key enhancements for the CBT Graph Viewer application. The features covered are derived from the project requirements and issue tracking log.

---

## 1. External API for Automation and Scripting (Req. 52, 51, 90)

**Objective:**
To provide an external API that allows programmatic control over the application for automation, scripting, and integration with other tools.

**Current Status:**
The application currently lacks any external API, forcing all interactions to be manual through the UI. This is tracked in issues #11, #10, and #45.

**Proposed Solution:**
We will implement a command-line interface (CLI) API. This approach avoids external library dependencies, aligns with the console-based nature of the application, and provides a powerful foundation for scripting.

*   **Phase 1: Core CLI Framework**
    *   Implement a simple, custom command-line argument parser to handle flags and values without external dependencies.
    *   Define a set of initial commands for core functionalities:
        *   `--load-graph <filepath>`: Load a graph from a CSV file.
        *   `--save-graph <filepath>`: Save the current graph to a CSV file.
        *   `--run-command <command>`: Execute an internal application command (e.g., 'pan-right', 'zoom-in').
        *   `--get-node-details <nodeId>`: Output details for a specific node in JSON format.
    *   Modify the application's entry point (`main` function) to handle CLI arguments and execute them, potentially in a non-interactive "headless" mode.

*   **Phase 2: Expanded Command Set**
    *   Add commands for graph manipulation: `add-node`, `remove-node`, `add-edge`, `remove-edge` (with parameters passed as arguments).
    *   Add commands for analytics: `get-degree <nodeId>`, `get-density`, `find-components`. These commands will invoke the modular analytics engine and return results as JSON.
    *   Add commands for exporting data: `export-analytics <type> <filepath>`.

*   **Phase 3: Scripting via CLI**
    *   True scripting language integration will be avoided to eliminate external dependencies. Instead, automation and "scripting" will be achieved by invoking the application's CLI from standard shell scripts (e.g., Bash, PowerShell).
    *   Users can chain commands to perform complex workflows. The JSON output from one command can be parsed by common shell tools (like `jq`) and used as input for a subsequent command.

**Dependencies:**
*   None for Phase 1.
*   Phase 2 depends on the completion of the Modular Analytics Engine.

**Testing Strategy:**
*   **Unit Tests:** Test the argument parser and individual command handlers.
*   **Integration Tests:** Create shell scripts that run the application with various CLI arguments and validate the output (e.g., file content, stdout JSON structure).

**Documentation:**
*   Create a new `docs/api_guide.md` detailing all available CLI commands, arguments, and examples.
*   Update the main `README.md` to mention the new CLI capabilities.

---

## 2. Modular Analytics & Data Interchange

**Objective:**
To refactor the analytics logic into a self-contained, modular component and implement a dependency-free JSON parser to standardize data exchange for the API and other features.

**Current Status:**
Analytics are currently hardcoded and tightly coupled with other application logic. There is no standard format for data interchange, and the project lacks JSON handling capabilities.

**Proposed Solution:**
*   **Phase 1: Analytics Engine Refactoring**
    *   Create a dedicated `AnalyticsEngine` class.
    *   Move all analytics functions (`getNodeDegree`, `computeDensity`, `findConnectedComponents`, etc.) into this class.
    *   Ensure these functions operate on graph data (`const Graph&`) and return results in plain C++ data structures, with no direct dependency on UI or rendering.

*   **Phase 2: Dependency-Free JSON Implementation**
    *   Implement JSON parsing and serialization functionality in `file_logic.h` and `file_logic.cpp` to avoid external libraries.
    *   This will involve:
        *   Creating a data structure (e.g., a `struct` or `class` using `std::variant` or a custom tagged union) to represent the JSON data model (object, array, string, number, boolean, null).
        *   Writing a recursive-descent parser that reads a `std::string` and populates this internal data structure.
        *   Writing a serializer that traverses the internal data structure and generates a valid JSON string.

*   **Phase 3: Integration**
    *   The CLI API will use the `AnalyticsEngine` to perform calculations.
    *   The results from the engine will be converted to the internal JSON representation and then serialized to a string for output to the console.
    *   This design ensures analytics are modular, testable, and easily exposed via the API in a standard format.

**Dependencies:**
*   None.

**Testing Strategy:**
*   **Unit Tests:** Test each function in the `AnalyticsEngine`.
*   **Integration Tests:** Create extensive unit tests for the JSON parser and serializer, covering valid inputs, edge cases, and malformed data.

**Documentation:**
*   Document the `AnalyticsEngine`'s public interface.
*   Document the JSON data structures and the public API of the parser/serializer for internal developers.

---

## 3. Logging Framework

**Objective:**
To implement a structured logging system for debugging, monitoring application health, and tracking user actions.

**Current Status:**
No formal logging system exists. Debugging relies on `std::cout` statements or a debugger.

**Proposed Solution:**
We will implement a simple, custom, header-only logging utility to avoid external dependencies.

*   **Phase 1: Setup and Basic Logging**
    *   Create a `Logger` singleton class to provide a global access point for logging.
    *   It will manage a `std::ofstream` for file logging to a configurable path (e.g., `logs/app.log`).
    *   Configure two output streams: A file stream for persistent logging and a console stream (`std::cout`/`std::cerr`) for real-time output, which can be enabled/disabled by a log level setting.
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

## 4. Unit & Integration Testing (Req. 57, 58)

**Objective:**
To establish a robust testing framework and increase test coverage to ensure code quality, prevent regressions, and facilitate safer refactoring.

**Current Status:**
Unit and integration test coverage is incomplete, as noted in issues #12 and #13.

**Proposed Solution:**
We will adopt a lightweight, custom testing approach using standard C++ features to avoid external frameworks.

*   **Phase 1: Framework Setup and Core Logic Tests**
    *   Create a separate `tests` directory and a main test runner executable.
    *   Write simple test functions for each module or class.
    *   Use the standard `assert()` macro or create custom assertion macros (e.g., `ASSERT_EQUAL(a, b)`) for checking conditions. Test failures will terminate the program or print a detailed error message to `std::cerr`.
    *   Prioritize writing unit tests for critical, non-UI logic:
        *   `Graph` class methods (add/remove nodes/edges, validation).
        *   The `AnalyticsEngine` and its functions.
        *   The custom JSON parser.
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

## 5. Performance Benchmarking (Req. 59)

**Objective:**
To measure, track, and optimize the performance of critical application functions, especially for large graphs.

**Current Status:**
Performance is unmeasured, and potential bottlenecks are unknown, as tracked in issue #14.

**Proposed Solution:**
We will use the standard C++ `<chrono>` library to create a dedicated benchmarking suite, avoiding external dependencies.

*   **Phase 1: Setup and Core Benchmarks**
    *   Integrate the benchmark runner into the build system as a separate executable.
    *   Create a `benchmarks` directory.
    *   Implement benchmarks for:
        *   The JSON parser with large input files.
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
*   Create a `docs/analytics.md` explaining how to build and run the benchmarks and documenting significant optimizations.
*   Benchmark results will be appended to `logs/performance.log` to track performance over time.

---

## 6. Code and API Documentation (Req. 60)

**Objective:**
To create comprehensive documentation for the codebase and its public API, making it easier for developers and users to understand and contribute.

**Current Status:**
Code documentation is sparse and inconsistent, as noted in issue #15.

**Proposed Solution:**
We will adopt a manual documentation approach using Markdown files and enforce a consistent in-code commenting style. This avoids external documentation generator tools.

*   **Phase 1: In-Code Commenting Standard**
    *   Establish a clear and consistent style for commenting functions, classes, and complex logic blocks directly within the source code.
    *   Go through the entire codebase and add comments explaining the purpose of each function, its parameters, and what it returns.

*   **Phase 2: Developer and API Documentation**
    *   Create a `docs/developer_guide.md` file to describe the high-level architecture, major components (`AnalyticsEngine`, `Graph`, `Renderer`), and the build process.
    *   The CLI API documentation, previously defined as `docs/api_guide.md`, will serve as the primary user-facing API document. A template will be created to ensure all commands are documented consistently (command, parameters, output format, examples).

**Dependencies:**
*   None.

**Documentation:**
*   This task is about creating documentation. The output will be the `developer_guide.md`, a populated `api_guide.md`, and improved in-code comments.
*   Add a section to the main `README.md` pointing to the new documentation files.

---

## 7. 3D Graph Rendering (Req. 71)

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
