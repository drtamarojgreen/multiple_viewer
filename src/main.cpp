#include "map_logic.h"
#include <queue>
#include <algorithm>
#include <chrono>
#include "viewer_logic.h"
#include "search_logic.h"
#include <iostream>
#include "../tests/test_logic.h"
#include "../tests/testsuite2_logic.h"
#include "../tests/testsuite3_logic.h"
#include "../tests/dynamic_graph_tests.h"
#include "../tests/bdd/bdd_test_main.h"
#include "file_logic.h" // Explicitly include for file operations
#include <string>
#include <vector>
#include "cmd_line_parser.h"
#include "model/model_repository.h"

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
int _getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

// Encapsulates the original interactive session logic
void runInteractiveSession(const CmdLineParser& parser) {
    //runAllTests();
    //runAll2Tests();
    if (!Config::quietMode) {
        std::cout<<"Press any key to continue...";
        _getch();
    }
    std::cout<<"\n";
    Graph graph;

    std::cout << "=== CBT Graph Editor ===\n";
    std::string inputFile = "graph_input.csv";
    if (parser.hasOption("load-graph")) {
        inputFile = parser.getOption("load-graph");
    } else if (parser.hasOption("load")) { // Fallback for old flag
        inputFile = parser.getOption("load");
    }

    std::cout << "Loading graph from '" << inputFile << "'...\n";

    // Initial load 
    if (!loadGraphFromCSV(graph, inputFile)) {
        std::cout << "Starting with empty graph.\n";
    }

    // Load brain model if specified
    if (parser.hasOption("load-atlas")) {
        model::ModelRepository::getInstance().loadAtlas(parser.getOption("load-atlas"));
    }
    if (parser.hasOption("load-labels")) {
        model::ModelRepository::getInstance().loadLabels(parser.getOption("load-labels"));
    }
    if (parser.hasOption("load-overlay")) {
        model::ModelRepository::getInstance().loadOverlay(parser.getOption("load-overlay"));
    }

    // Run viewer/editor session
    runEditor(graph, parser.hasOption("test"));
}

int runApplication(const CmdLineParser& parser) {
    if (parser.hasOption("test") || parser.hasOption("test-unit")) {
        runAllTests();
        runAll2Tests();
        runAll3Tests();
        runDynamicGraphTests();
        if (parser.hasOption("test-unit") && !parser.hasOption("test-bdd")) return 0;
    }

    if (parser.hasOption("test") || parser.hasOption("test-bdd")) {
        runBDDTests();
        return 0;
    }

    if (parser.hasOption("help")) {
        std::cout << "Usage: viewer [options]\n";
        std::cout << "Options:\n";
        std::cout << "  --load-graph <file.csv>   Load graph from CSV\n";
        std::cout << "  --save-graph <file.csv>   Save graph to CSV (headless)\n";
        std::cout << "  --get-node-details <id>  Print node details and exit\n";
        std::cout << "  --load-atlas <file.brn>   Load brain atlas\n";
        std::cout << "  --load-labels <file.txt>  Load brain labels\n";
        std::cout << "  --load-overlay <file.txt> Load node-to-brain overlay\n";
        std::cout << "  --test-unit               Run unit tests\n";
        std::cout << "  --test-bdd                Run BDD tests\n";
        std::cout << "  --test                    Run all tests\n";
        std::cout << "  --help                    Show this help\n";
        return 0;
    }

    // Headless operations
    if (parser.hasOption("load-graph") || parser.hasOption("save-graph") || parser.hasOption("get-node-details")) {
        Graph graph;
        std::string loadPath = parser.getOption("load-graph");
        if (loadPath.empty()) loadPath = "graph_input.csv";

        if (!loadGraphFromCSV(graph, loadPath)) {
            std::cerr << "Error: Could not load graph from " << loadPath << "\n";
            return 1;
        }

        if (parser.hasOption("get-node-details")) {
            int id = std::stoi(parser.getOption("get-node-details"));
            if (graph.nodeExists(id)) {
                const GraphNode& node = graph.nodeMap.at(id);
                std::cout << "Node Index: " << node.index << "\n";
                std::cout << "Label: " << node.label << "\n";
                std::cout << "Weight: " << node.weight << "\n";
                std::cout << "Subject Index: " << node.subjectIndex << "\n";
                std::cout << "Neighbor Count: " << node.neighbors.size() << "\n";
            } else {
                std::cout << "Node with index " << id << " not found.\n";
            }
        }

        if (parser.hasOption("save-graph")) {
            std::string savePath = parser.getOption("save-graph");
            if (saveGraphToCSV(graph, savePath)) {
                std::cout << "Graph saved to " << savePath << "\n";
            } else {
                std::cerr << "Error: Could not save graph to " << savePath << "\n";
                return 1;
            }
        }

        // If we did headless ops, don't start interactive session unless specifically requested?
        // Usually CLI flags mean headless.
        return 0;
    }

    runInteractiveSession(parser);
    return 0;
}

int main(int argc, char* argv[]) {
    CmdLineParser parser(argc, argv);
    return runApplication(parser);
}