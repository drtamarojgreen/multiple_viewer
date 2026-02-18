#include "map_logic.h"
#include <queue>
#include <algorithm>
#include <chrono>
#include "viewer_logic.h"
#include "search_logic.h"
#include <iostream>
#include "test_logic.h"
#include "testsuite2_logic.h"
#include "testsuite3_logic.h"
#include "file_logic.h" // Explicitly include for file operations
#include <string>
#include <vector>
#include "cmd_line_parser.h"

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
}

int runApplication(const CmdLineParser& parser) {
    if (parser.hasOption("test")) {
        runAllTests();
        runAll2Tests();
        runAll3Tests();
        return 0;
    }

    if (parser.hasOption("help")) {
        std::cout << "Usage: viewer [--load <file.csv>] [--test] [--help]\n";
        return 0;
    }

    runInteractiveSession(parser);
    return 0;
}

int main(int argc, char* argv[]) {
    CmdLineParser parser(argc, argv);
    return runApplication(parser);
}