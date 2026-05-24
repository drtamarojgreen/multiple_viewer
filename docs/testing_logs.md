# Testing Logs

## 2026-05-22
- **Unified Test Run**:
  - Unit Tests: 23/23 Passed (100%)
  - BDD Features: All primary features passed.
  - SDD Cards: 15/15 Verified (including new Thread-safety and Config cards).
- **Major Fixes**:
  - Resolved `unordered_map::at` crash in unit tests by restoring valid `test_graph.csv`.
  - Fixed multiple definition errors for `Config` variables by moving from `inline` to `extern`.
  - Resolved thread-safety race conditions in `Graph` by implementing an internal mutex.
- **New Features Verified**:
  - MeSH Discovery Engine with XML extraction logic.
  - Heuristic NLP keyword/entity extraction.
  - Logistic S-curve growth model calculation.
  - XML-driven command mapping system.
  - YAML-based configuration management.
  - Crash recovery via periodic autosave.
