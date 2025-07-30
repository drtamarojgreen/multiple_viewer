#include "map_logic.h"
#include "viewer_logic.h"
#include <iostream>
#include "test_logic.h"
#include "testsuite2_logic.h"
#include "file_logic.h" // Explicitly include for file operations
#include <string>
#include <vector>

#include <conio.h>

// Encapsulates the original interactive session logic
void runInteractiveSession() {
    //runAllTests();
    //runAll2Tests();
    std::cout<<"Press any key to continue...";
    _getch();
    std::cout<<"\n";
    Graph graph;

    std::cout << "=== CBT Graph Editor ===\n";
    std::cout << "Loading graph from 'graph_input.csv'...\n";

    // Initial load 
    if (!loadGraphFromCSV(graph, "graph_input.csv")) {
        std::cout << "Starting with empty graph.\n";
    }

    // Run viewer/editor session
    runEditor(graph);

    std::cout << "Session complete. Saving to 'graph_output.csv'...\n";
    saveGraphToCSV(graph, "graph_output.csv");

    std::cout << "Goodbye.\n";
}

int main(int argc, char* argv[]) {
    // If no command-line arguments are provided, run the default interactive session.
    if (argc < 2) {
        runInteractiveSession();
        return 0;
    }

    // --- Command-Line Interface Mode ---
    std::cout << "Running in command-line mode..." << std::endl;
    std::vector<std::string> args(argv + 1, argv + argc);

    // In the next step, we will parse these arguments.
    // For now, we just print them to confirm the framework is in place.
    for (size_t i = 0; i < args.size(); ++i) {
        std::cout << "Argument " << i << ": " << args[i] << std::endl;
    }

    std::cout << "CLI execution finished." << std::endl;

    return 0;
}
