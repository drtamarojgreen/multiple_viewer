# Brain Model Integration Guide

This guide describes the modeling infrastructure integrated into the CBT Graph Viewer suite.

## 1. Core Simulation Kernel

The `SimulationKernel` is a deterministic engine responsible for time-stepping.

- **Determinism:** Results are guaranteed to be identical for a given seed.
- **Persistence:** Supports `capture_snapshot` and `restore_snapshot` to a `SimulationSnapshot` state blob.

## 2. Brain Model Registry

The `BrainModelRegistry` serves as a central hub for multiple modeling families.

- **Models:** Each `BrainModel` contains a set of `BrainRegion` and `BrainPathway` objects.
- **Hierarchies:** Regions support parent-child relationships (e.g., "Hippocampus" inside "Limbic Lobe").
- **Spatial Indexing:** Regions are automatically indexed into an Octree to support fast O(log N) proximity and ROI queries.

## 3. Temporal Engine

The `TemporalEngine` manages time-series data playback.

- **Activity Mapping:** Can map activity samples to specific nodes or regions over time.
- **Interpolation:** Automatically computes intermediate values between discrete time frames for smooth playback.

## 4. Overlay Service

The `OverlayService` manages analytical and status overlays that provide real-time feedback during simulation.

- **Prioritization:** Overlays can have different priority levels.
- **Anchoring:** Overlays can be anchored globally or to specific brain entities (nodes or regions).
- **Metadata:** Includes roles (e.g., "Clinical", "Telemetry") to filter feedback for different research personas.
