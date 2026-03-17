CXX = g++
CXXFLAGS = -std=c++17 -I../../src -I../../src/model -I../../src/io -I../../src/analytics -I../../src/layout -I../../src/render -I../../src/ui -I../../src/input -I../../src/scripting -I. -I../cpp/util
LDFLAGS =

BUILD_DIR = ../../build/cdd
BIN_DIR = bin

# Core objects from the main project
CORE_OBJS = ../../build/src/map_logic.o \
            ../../build/src/viewer_logic.o \
            ../../build/src/search_logic.o \
            ../../build/src/file_logic.o \
            ../../build/src/io/io_manager.o \
            ../../build/src/analytics/analytics_engine_ext.o \
            ../../build/src/layout/layout_manager.o \
            ../../build/src/console_logic.o \
            ../../build/src/cmd_line_parser.o

# CDD Cards
CARDS = cards/LoggingClass.cpp \
        cards/EvaluatorsClass.cpp \
        cards/QuantaGliaClass.cpp \
        cards/PrunerClass.cpp \
        cards/ViewportClass.cpp \
        cards/LayoutClass.cpp

BINS = $(patsubst cards/%.cpp, $(BIN_DIR)/%, $(CARDS))

all: $(BINS)

$(BIN_DIR)/%: cards/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< $(CORE_OBJS) -o $@ $(LDFLAGS)

clean:
	rm -rf $(BIN_DIR)

test: all
	@echo "Running CDD Tests..."
	@for bin in $(BINS); do \
		echo "Executing $$bin..."; \
		./$$bin; \
	done

.PHONY: all clean test
