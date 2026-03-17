CXX = g++
CXXFLAGS = -std=c++17 -I../../src -I../../src/model -I../../src/io -I../../src/analytics -I../../src/layout -I../../src/render -I../../src/ui -I../../src/input -I../../src/scripting -I. -I../cpp/util
LDFLAGS =

BUILD_DIR = ../../build
BIN_DIR = bin

# Automatically find all core objects built by the main Makefile
CORE_OBJS = $(shell find $(BUILD_DIR)/src -name "*.o")

# CDD Cards
CARDS = cards/LoggingClass.cpp \
        cards/EvaluatorsClass.cpp \
        cards/QuantaGliaClass.cpp \
        cards/PrunerClass.cpp \
        cards/ViewportClass.cpp \
        cards/LayoutClass.cpp

BINS = $(patsubst cards/%.cpp, $(BIN_DIR)/%, $(CARDS))

# Optional facts directory (can be overridden on command line)
FACTS_DIR ?= ../../facts

all: $(BINS)

$(BIN_DIR)/%: cards/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< $(CORE_OBJS) -o $@ $(LDFLAGS)

clean:
	rm -rf $(BIN_DIR)

test: all
	@echo "Running CDD Tests with CHAI_FACTS_DIR=$(FACTS_DIR)..."
	@for bin in $(BINS); do \
		echo "Executing $$bin..."; \
		CHAI_FACTS_DIR=$(FACTS_DIR) ./$$bin; \
	done

.PHONY: all clean test
