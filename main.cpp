#include "map_logic.h"
#include "viewer_logic.h"
#include <iostream>
#include "test_logic.h"
#include "testsuite2_logic.h"
#include "cmd_line_parser.h"

#include <conio.h>

int runApplication(CmdLineParser& parser) {
    if (parser.hasOption("quiet")) {
        Config::quietMode = true;
    }

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
    if (parser.hasOption("load")) {
        inputFile = parser.getOption("load");
    }

    std::cout << "Loading graph from '" << inputFile << "'...\n";

    // Initial load 
    if (!loadGraphFromCSV(graph, inputFile)) {
        std::cout << "Starting with empty graph.\n";
    }

    // Run viewer/editor session
    runEditor(graph, parser.hasOption("test"));

    std::cout << "Session complete. Saving to 'graph_output.csv'...\n";
    saveGraphToCSV(graph, "graph_output.csv");

    std::cout << "Goodbye.\n";
    return 0;
}

int main(int argc, char* argv[]) {
    CmdLineParser parser(argc, argv);
    return runApplication(parser);
}
