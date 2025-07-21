#include "map_logic.h"
#include "viewer_logic.h"
#include "search_logic.h"
#include <iostream>
#include "test_logic.h"
#include "testsuite2_logic.h"

int main() {
    //runAllTests();
    //runAll2Tests();
    std::cout<<"Press any key to continue...";
    std::cin.get();
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
    return 0;
}
