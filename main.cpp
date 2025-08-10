#include "map_logic.h"
#include "viewer_logic.h"
#include <iostream>
#include "test_logic.h"
#include "testsuite2_logic.h"
#include "file_logic.h" // Explicitly include for file operations
#include <string>
#include <vector>

#include <conio.h>

// New struct to hold a parsed command-line argument
struct CliCommand {
    std::string command;
    std::string value;
};

// Function to parse arguments into a list of commands
std::vector<CliCommand> parseCliArgs(const std::vector<std::string>& args) {
    std::vector<CliCommand> commands;
    for (size_t i = 0; i < args.size(); ++i) {
        const std::string& arg = args[i];
        if (arg.rfind("--", 0) == 0) { // Check if it's a command flag
            CliCommand cmd;
            cmd.command = arg;
            // Check if there is a next argument and it's not another command
            if (i + 1 < args.size() && args[i + 1].rfind("--", 0) != 0) {
                cmd.value = args[i + 1];
                i++; // Skip the value in the next iteration
            }
            commands.push_back(cmd);
        }
    }
    return commands;
}


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

// Function to process the parsed command-line commands
void processCliCommands(const std::vector<CliCommand>& commands) {
    Graph graph;
    bool graph_loaded = false;

    // First pass: handle --load-graph to ensure it runs first.
    for (const auto& cmd : commands) {
        if (cmd.command == "--load-graph") {
            if (!cmd.value.empty()) {
                if (loadGraphFromCSV(graph, cmd.value)) {
                    std::cout << "Graph loaded successfully from " << cmd.value << std::endl;
                    graph_loaded = true;
                } else {
                    std::cerr << "Error: Failed to load graph from " << cmd.value << std::endl;
                }
            } else {
                std::cerr << "Error: --load-graph requires a filepath." << std::endl;
            }
            break; // Process only the first --load-graph command
        }
    }

    if (!graph_loaded) {
        std::cout << "Starting with an empty graph as --load-graph was not specified." << std::endl;
    }

    // Second pass: process all other commands
    for (const auto& cmd : commands) {
        if (cmd.command == "--load-graph") {
            // Already handled in the first pass
            continue;
        } else if (cmd.command == "--save-graph") {
            if (!cmd.value.empty()) {
                if (saveGraphToCSV(graph, cmd.value)) {
                    std::cout << "Graph saved successfully to " << cmd.value << std::endl;
                } else {
                    std::cerr << "Error: Failed to save graph to " << cmd.value << std::endl;
                }
            } else {
                std::cerr << "Error: --save-graph requires a filepath." << std::endl;
            }
        } else if (cmd.command == "--get-node-details") {
            if (!cmd.value.empty()) {
                try {
                    int nodeId = std::stoi(cmd.value);
                    std::string json_output = serializeNodeToJson(graph, nodeId);
                    std::cout << json_output << std::endl;
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Error: Invalid node ID for --get-node-details." << std::endl;
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error: Node ID for --get-node-details is out of range." << std::endl;
                }
            } else {
                std::cerr << "Error: --get-node-details requires a node ID." << std::endl;
            }
        } else if (cmd.command == "--run-command") {
            if (!cmd.value.empty()) {
                executeGraphCommand(graph, cmd.value);
            } else {
                std::cerr << "Error: --run-command requires a command string." << std::endl;
            }
        } else {
            std::cerr << "Error: Unknown command: " << cmd.command << std::endl;
        }
    }
}


int main(int argc, char* argv[]) {
    runAllTests();
    runAll2Tests();

    // If no command-line arguments are provided, run the default interactive session.
    if (argc < 2) {
        runInteractiveSession();
        return 0;
    }

    // --- Command-Line Interface Mode ---
    std::cout << "Running in command-line mode..." << std::endl;
    std::vector<std::string> args(argv + 1, argv + argc);
    std::vector<CliCommand> commands = parseCliArgs(args);

    processCliCommands(commands);

    std::cout << "CLI execution finished." << std::endl;

    return 0;
}
