# CHAI Checkins - Converted Tests Workspace

- quanta_glia_extraction_sip: Compile and execute `QuantaGliaClass.cpp` to verify graph IO logic. (Converted to multiple_viewer)
- pruner_logic_sip: Compile and execute `PrunerClass.cpp` to verify graph analytics logic. (Converted to multiple_viewer)
- logging_setup_sip: Compile and execute `LoggingClass.cpp` to verify C++ Logger logic. (Converted to multiple_viewer)
- evaluator_logic_sip: Compile and execute `EvaluatorsClass.cpp` to verify search logic. (Converted to multiple_viewer)
- viewport_verification_sip: Compile and execute `ViewportClass.cpp` to verify navigation logic. (New for multiple_viewer)
- layout_verification_sip: Compile and execute `LayoutClass.cpp` to verify layout algorithm logic. (New for multiple_viewer)
- cdd_makefile_sip: Created `tests/cdd/Makefile` to automate CDD test compilation and execution.
- cdd_makefile_deps_sip: Updated `tests/cdd/Makefile` to include all core objects for proper linking.
- cdd_makefile_recursive_sip: Refined `tests/cdd/Makefile` to recursively link all core objects, resolving nested dependency issues.
- layout_card_fix_sip: Fixed type mismatch in `LayoutClass.cpp` (Coord3 vs Point3D).
- cdd_runtime_fix_sip: Fixed fact file paths and adjusted layout test for current stubbed implementation.
- fact_loading_robustness_sip: Enhanced `FactReader` with automatic path redirection to handle different execution contexts.
- fact_loading_configurable_sip: Implemented `CHAI_FACTS_DIR` env support and heuristic search for fact files.
