cmake_minimum_required(VERSION 3.10)
project(MultipleViewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define project root for resource path handling
add_definitions(-DPROJECT_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# Include directories
include_directories(src)
include_directories(tests)

# Explicitly list source files to avoid GLOB shortcuts and preserve structure visibility in IDEs
set(CORE_SOURCES
    src/main.cpp
    src/map_logic.cpp
    src/viewer_logic.cpp
    src/file_logic.cpp
    src/search_logic.cpp
    src/cmd_line_parser.cpp
)

set(MODEL_SOURCES
    src/model/brain_model.cpp
    src/model/brain_label.cpp
    src/model/brain_overlay.cpp
    src/model/model_repository.cpp
)

set(RENDER_SOURCES
    src/render/console_renderer.cpp
    src/render/sdl_renderer.cpp
    src/render/camera.cpp
    src/render/minimap_renderer.cpp
    src/render/visual_mapper.cpp
)

set(INPUT_SOURCES
    src/input/input_manager.cpp
    src/input/shortcut_manager.cpp
    src/input/command_stack.cpp
)

# New Enhancement Modules (Architectural Foundations)
set(EXT_SOURCES
    src/io/io_manager.cpp
    src/analytics/analytics_engine_ext.cpp
    src/analytics/temporal_manager.cpp
    src/scripting/script_runtime.cpp
    src/scripting/plugin_manager.cpp
    src/layout/layout_manager.cpp
    src/ui/help_system.cpp
    src/ui/benchmark_runner.cpp
    src/render/spatial_index.cpp
)

set(TEST_SOURCES
    tests/test_logic.cpp
    tests/testsuite2_logic.cpp
    tests/testsuite3_logic.cpp
    tests/dynamic_graph_tests.cpp
)

set(BDD_SOURCES
    tests/bdd/bdd_runner.cpp
    tests/bdd/bdd_test_main.cpp
    tests/bdd/steps/brain_steps.cpp
    tests/bdd/steps/rendering_steps.cpp
    tests/bdd/steps/data_steps.cpp
    tests/bdd/steps/automation_steps.cpp
    tests/bdd/steps/original_steps.cpp
)

# Combine all sources
set(ALL_SOURCES
    ${CORE_SOURCES}
    ${MODEL_SOURCES}
    ${RENDER_SOURCES}
    ${INPUT_SOURCES}
    ${EXT_SOURCES}
    ${TEST_SOURCES}
    ${BDD_SOURCES}
)

add_executable(multiple_viewer ${ALL_SOURCES})

# Target-specific include directories
target_include_directories(multiple_viewer PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

# Future dependencies
# find_package(SDL2 REQUIRED)
# target_link_libraries(multiple_viewer SDL2::SDL2)

# Installation logic
install(TARGETS multiple_viewer RUNTIME DESTINATION bin)
