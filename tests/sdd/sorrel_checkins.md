# SORREL Checkins - Converted Tests Workspace

- quanta_glia_extraction_sip: Compile and execute `QuantaGliaClass.cpp` to verify graph IO logic. (Converted to multiple_viewer)
- pruner_logic_sip: Compile and execute `PrunerClass.cpp` to verify graph analytics logic. (Converted to multiple_viewer)
- logging_setup_sip: Compile and execute `LoggingClass.cpp` to verify C++ Logger logic. (Converted to multiple_viewer)
- evaluator_logic_sip: Compile and execute `EvaluatorsClass.cpp` to verify search logic. (Converted to multiple_viewer)
- viewport_verification_sip: Compile and execute `ViewportClass.cpp` to verify navigation logic. (New for multiple_viewer)
- layout_verification_sip: Compile and execute `LayoutClass.cpp` to verify layout algorithm logic. (New for multiple_viewer)
- sdd_makefile_sip: Created `tests/sdd/Makefile` to automate SDD test compilation and execution.
- sdd_makefile_deps_sip: Updated `tests/sdd/Makefile` to include all core objects for proper linking.
- sdd_makefile_recursive_sip: Refined `tests/sdd/Makefile` to recursively link all core objects, resolving nested dependency issues.
- layout_card_fix_sip: Fixed type mismatch in `LayoutClass.cpp` (Coord3 vs Point3D).
- sdd_runtime_fix_sip: Fixed fact file paths and adjusted layout test for current stubbed implementation.
- fact_loading_robustness_sip: Enhanced `FactReader` with automatic path redirection to handle different execution contexts.
- fact_loading_configurable_sip: Implemented `SORREL_FACTS_DIR` env support and heuristic search for fact files.
- mesh_json_ingestion_sip: Implemented `loadMeshJSON` in `IOManager` for Greenhouse pipeline integration.
- analytics_worker_pool_sip: Implemented `WorkerPool` for concurrent analytics tasks.
- io_abstraction_sip: Implemented `StorageBackend` and `LocalFS` for distributed file system support.
