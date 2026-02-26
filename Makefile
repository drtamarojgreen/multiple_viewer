CXX = g++
BUILD_DIR = build
TARGET = $(BUILD_DIR)/multiple_viewer

# Project Root Detection (Absolute path to the directory containing this Makefile)
PROJECT_ROOT = $(shell pwd)
CXXFLAGS = -std=c++17 -Isrc -Itests -DPROJECT_ROOT_DIR=\"$(PROJECT_ROOT)\"
LDFLAGS = 

# Explicit list of source directories (mirroring the modular structure)
SRC_DIRS = src src/model src/model/core src/model/core/contracts src/model/domains src/model/domains/emotion src/model/domains/stress src/model/domains/molecular src/model/domains/neuro src/model/domains/neurochemical src/model/app src/render src/input src/io src/analytics src/scripting src/layout src/ui tests tests/bdd tests/bdd/steps

# Discover all source files across modular packages
SRC_FILES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))

# Generate object file paths inside the BUILD_DIR
# This preserves the directory structure within build/
OBJ_FILES = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to compile .cpp to .o inside BUILD_DIR
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

test: $(TARGET)
	./$(TARGET) --test

.PHONY: all clean test
