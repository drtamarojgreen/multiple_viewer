CXX = g++
BUILD_DIR = build
CXXFLAGS = -std=c++17 -Isrc -Itests -Itests/bdd -DPROJECT_ROOT_DIR=\"$(shell pwd)\"
LDFLAGS = 

# Source directories for core logic
CORE_DIRS = src src/model src/model/core src/model/app src/render src/input src/io src/analytics src/scripting src/layout src/ui
CORE_FILES = $(foreach dir,$(CORE_DIRS),$(wildcard $(dir)/*.cpp))
CORE_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(CORE_FILES))

# Test source directories (excluding mains)
TEST_CORE_DIRS = tests tests/bdd tests/bdd/steps
TEST_CORE_FILES = $(wildcard tests/test*.cpp) tests/dynamic_graph_tests.cpp tests/bdd/bdd_runner.cpp tests/bdd/bdd_test_main.cpp $(wildcard tests/bdd/steps/*.cpp) $(wildcard tests/print/*.cpp)
TEST_CORE_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(TEST_CORE_FILES))

# Targets
VIEWER_TARGET = $(BUILD_DIR)/viewer
BRAIN_DEMO_TARGET = $(BUILD_DIR)/brain_demo
UNIT_TESTS_TARGET = $(BUILD_DIR)/unit_tests
BDD_TESTS_TARGET = $(BUILD_DIR)/bdd_tests

all: $(VIEWER_TARGET) $(BRAIN_DEMO_TARGET) $(UNIT_TESTS_TARGET) $(BDD_TESTS_TARGET)

# Viewer executable
$(VIEWER_TARGET): $(BUILD_DIR)/apps/viewer/main.o $(CORE_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Brain Demo executable
$(BRAIN_DEMO_TARGET): $(BUILD_DIR)/apps/brain_demo/main.o $(CORE_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Unit Tests executable
$(UNIT_TESTS_TARGET): $(BUILD_DIR)/tests/unit_main.o $(CORE_OBJS) $(TEST_CORE_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# BDD Tests executable
$(BDD_TESTS_TARGET): $(BUILD_DIR)/tests/bdd/bdd_main.o $(CORE_OBJS) $(TEST_CORE_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compilation rules
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

test: $(UNIT_TESTS_TARGET) $(BDD_TESTS_TARGET)
	./$(UNIT_TESTS_TARGET)
	./$(BDD_TESTS_TARGET)

.PHONY: all clean test
