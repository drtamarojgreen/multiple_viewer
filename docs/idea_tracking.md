# Idea Tracking

This document tracks larger ideas that may become roadmap items.

## Plan: Integrate research mesh results into the viewer

Reference source for generated artifacts and scripts:
- `greenhouse_org/scripts/research/mesh` in the external repository.

### Goal
Surface mesh-tooling outputs (nodes, relationships, clusters, and confidence/metadata) directly in `multiple_viewer` so users can inspect and navigate research-derived structures alongside the existing graph workflows.

### Phase 1 — Discovery and contract definition
1. Inventory mesh output files produced by the research scripts (JSON/CSV/schema/version fields).
2. Define a stable import contract in this project:
   - Required fields (id, label/title, links/edges).
   - Optional fields (cluster/topic, confidence, source refs, timestamps).
   - Versioning field to protect against schema drift.
3. Add a short `docs/mesh_import_contract.md` with examples and failure behavior.

### Phase 2 — Import pipeline in `multiple_viewer`
1. Add a parser module (e.g., `src/import/mesh_importer.*`) that transforms mesh output into `GraphNode` + edge updates.
2. Implement strict + permissive modes:
   - Strict mode fails fast on invalid required fields.
   - Permissive mode imports valid records and logs skipped rows.
3. Add command-line support for loading mesh artifacts at startup (path + mode flags).

### Phase 3 — Viewer UX integration
1. Add visual cues for mesh metadata:
   - Confidence bands (high/med/low styling).
   - Cluster/group indicators.
   - Optional source marker icon/text in node detail/page view.
2. Add filter and search extensions:
   - Filter by cluster.
   - Filter by confidence threshold.
   - Search in mesh-derived metadata fields.
3. Ensure view modes (Perspective, Nexus Flow, Book) can render imported mesh data without layout regressions.

### Phase 4 — Validation and quality gates
1. Unit tests for parser behavior against valid/invalid fixture files.
2. Integration tests for end-to-end import + render smoke path.
3. Regression checks for existing CSV workflows to ensure mesh support is additive.

### Phase 5 — Operationalization
1. Add example mesh fixture(s) in `data/` and document usage in `docs/testing_guide.md`.
2. Add logging metrics (import counts, skipped records, parse duration).
3. Add a follow-up task for incremental refresh if mesh outputs are regenerated frequently.

### Risks / open questions
- Schema volatility in the external mesh scripts may require a version adapter layer.
- Large mesh datasets may require lazy loading or pagination in book/page views.
- Need decision on whether mesh confidence should influence physics/layout weights.
