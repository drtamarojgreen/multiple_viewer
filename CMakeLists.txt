cmake_minimum_required(VERSION 3.10)
project(MultipleViewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(PROJECT_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# Include directories mirrored from Makefile flags.
include_directories(
    src
    tests
    tests/bdd
)

# Source directories for core logic (mirrors Makefile CORE_DIRS).
set(CORE_DIRS
    src
    src/model
    src/model/core
    src/model/app
    src/render
    src/input
    src/io
    src/analytics
    src/scripting
    src/layout
    src/ui
)

set(CORE_SOURCES)
foreach(dir ${CORE_DIRS})
    file(GLOB DIR_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/*.cpp")
    list(APPEND CORE_SOURCES ${DIR_SOURCES})
endforeach()

# Test source files excluding test mains (mirrors Makefile TEST_CORE_FILES).
file(GLOB TEST_PREFIX_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/test*.cpp")
file(GLOB BDD_STEP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/bdd/steps/*.cpp")
file(GLOB PRINT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/print/*.cpp")

set(TEST_CORE_SOURCES
    ${TEST_PREFIX_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/dynamic_graph_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/bdd/bdd_runner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/bdd/bdd_test_main.cpp
    ${BDD_STEP_SOURCES}
    ${PRINT_SOURCES}
)

set(VIEWER_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/apps/viewer/main.cpp)
set(UNIT_TEST_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit_main.cpp)
set(BDD_TEST_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/tests/bdd/bdd_main.cpp)

# Targets aligned with Makefile outputs.
add_executable(viewer
    ${VIEWER_MAIN}
    ${CORE_SOURCES}
)

add_executable(unit_tests
    ${UNIT_TEST_MAIN}
    ${CORE_SOURCES}
    ${TEST_CORE_SOURCES}
)

add_executable(bdd_tests
    ${BDD_TEST_MAIN}
    ${CORE_SOURCES}
    ${TEST_CORE_SOURCES}
)

install(TARGETS viewer unit_tests bdd_tests RUNTIME DESTINATION bin)
