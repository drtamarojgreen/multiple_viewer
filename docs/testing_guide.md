# Testing Guide

The CBT Graph Viewer suite employs a multi-tiered testing strategy to ensure both technical correctness and behavioral validity.

## 1. Unit Tests

Located in `tests/`, these tests verify individual components in isolation.

- **Execution:** `./build/unit_tests`
- **Scope:**
  - Graph theory algorithms (Shortest Path, Connectivity)
  - Analytics calculations (Clustering, Centrality)
  - File I/O robustness
  - Spatial indexing performance (Octree)
  - Memory management and state clearing

## 2. BDD (Behavior-Driven Development) Tests

Located in `tests/bdd/`, these tests use the Gherkin syntax (GIVEN/WHEN/THEN) to describe high-level system behaviors.

- **Execution:** `./build/bdd_tests`
- **Key Feature Areas:**
  - **Core Kernel:** Determinism and snapshot/restore.
  - **Brain Model:** Region hierarchy and mapping integrity.
  - **UI/UX:** Navigation, zooming, and view mode transitions.
  - **Data Analytics:** Community detection and multi-format loading.

## 3. UIPrinter Framework

To facilitate automated testing of visual components, the suite includes a `UIPrinter` mock renderer. It implements the `IRenderer` interface but outputs to a text buffer instead of a screen.

- **Usage in BDD:** Scenarios can assert on strings in the `UIPrinter` buffer to verify that specific nodes or metadata (like pan/zoom offsets) are being logically "rendered" to the viewport.
- **Regression Testing:** Provides a stable way to verify that panning and culling logic remain correct as layout algorithms evolve.

## 4. Best Practices for New Tests

- **Unit Tests:** Always add a unit test for new utility functions or algorithm changes.
- **BDD Features:** When implementing a new user-facing feature (like a new hotkey or layout), add a corresponding `.feature` file.
- **Mocking:** Use the `MockSimulationKernel` or `MockOverlayService` classes in `domain_steps.cpp` for tests that require complex state without full simulation overhead.
