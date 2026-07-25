#include "map_logic.h"
#include <queue>
#include <algorithm>
#include <chrono>
#include <fstream>
#include "viewer_logic.h"
#include "render/console_renderer.h"
#include "search_logic.h"
#include <iostream>
#include "unit/test_logic.h"
#include "unit/testsuite2_logic.h"
#include "unit/testsuite3_logic.h"
#include "unit/dynamic_graph_tests.h"
#include "tests/bdd/bdd_test_main.h"
#include "io/io_manager.h" // Explicitly include for file operations
#include <string>
#include <vector>
#include "cmd_line_parser.h"
#include "model/model_repository.h"
#include "sdd_checker.h"
#include "analytics/mesh_discovery_engine.h"
#include "analytics/worker_pool.h"
#include "analytics/nlp_engine.h"
#include "analytics/trend_analyzer.h"
#include "genome/genome_manager.h"
#include "layout/layout_manager.h"
#include "processor_logic.h"
#include "analysis_logic.h"

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
    bool meshMode = false;
    if (parser.hasOption("load-mesh")) {
        inputFile = parser.getOption("load-mesh");
        meshMode = true;
    } else if (parser.hasOption("load-graph")) {
        inputFile = parser.getOption("load-graph");
    } else if (parser.hasOption("load")) { // Fallback for old flag
        inputFile = parser.getOption("load");
    }

    std::cout << "Loading graph from '" << inputFile << "'...\n";

    // Initial load 
    if (meshMode) {
        if (!io::IOManager::loadMeshJSON(graph, inputFile)) {
            std::cout << "Starting with empty graph (Mesh JSON failed).\n";
        }
    } else {
        if (!io::IOManager::loadGraphFromCSV(graph, inputFile)) {
            std::cout << "Starting with empty graph.\n";
        }
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
    // Initial configuration load
    Config::loadFromYaml("config/app_config.yaml");
    analytics::NlpEngine::loadConfig("config/mesh_config.yaml");
    analytics::TrendAnalyzer::loadConfig("config/mesh_config.yaml");

    if (parser.hasOption("test") || parser.hasOption("test-unit") || parser.hasOption("test-bdd")) {
        std::cout << "Tests are now available via separate executables: unit_tests and bdd_tests\n";
        return 0;
    }

    if (parser.hasOption("help")) {
        std::cout << "Usage: viewer [options]\n";
        std::cout << "Options:\n";
        std::cout << "  --load-graph <file.csv>   Load graph from CSV\n";
        std::cout << "  --load-mesh <file.json>    Load graph from Mesh JSON\n";
        std::cout << "  --discover-mesh <seed>     Run hierarchical MeSH discovery\n";
        std::cout << "  --genome-query <query>    Query Genome API and cache locally\n";
        std::cout << "  --save-graph <file.csv>   Save graph to CSV (headless)\n";
        std::cout << "  --get-node-details <id>  Print node details and exit\n";
        std::cout << "  --load-atlas <file.brn>   Load brain atlas\n";
        std::cout << "  --load-labels <file.txt>  Load brain labels\n";
        std::cout << "  --load-overlay <file.txt> Load node-to-brain overlay\n";
        std::cout << "  --check-sdd <repo>        Check repository for SDD adherence\n";
        std::cout << "  --process                 Process topics from source to target\n";
        std::cout << "  --source <dir>            Source directory for processing (default: source)\n";
        std::cout << "  --target <dir>            Target directory for processing (default: target)\n";
        std::cout << "  --summary                 Report graph summary data\n";
        std::cout << "  --filename <file.csv>     Filename for summary (default: graph.csv)\n";
        std::cout << "  --export-svg <file.svg>   Export graph to SVG (headless)\n";
        std::cout << "  --test-unit               Run unit tests\n";
        std::cout << "  --test-bdd                Run BDD tests\n";
        std::cout << "  --test                    Run all tests\n";
        std::cout << "  --help                    Show this help\n";
        return 0;
    }

    if (parser.hasOption("check-sdd")) {
        std::string repo = parser.getOption("check-sdd");
        SddChecker checker;
        SddCheckResult result = checker.checkRepository(repo);
        std::cout << result.message << std::endl;
        for (const auto& detail : result.details) {
            std::cout << "  - " << detail << std::endl;
        }
        return result.passed ? 0 : 1;
    }

    if (parser.hasOption("process")) {
        std::string source = parser.getOption("source");
        if (source.empty()) source = "source";
        std::string target = parser.getOption("target");
        if (target.empty()) target = "target";
        return tp::processTopics(source, target);
    }

    if (parser.hasOption("summary")) {
        std::string filename = parser.getOption("filename");
        if (filename.empty()) filename = "graph.csv";
        Graph graph;
        if (io::IOManager::loadGraphFromCSV(graph, filename)) {
            graph.updateSummary();
            AnalyticsEngine::printSummary(graph);
        } else {
            std::cerr << "Error: Could not load graph from " << filename << "\n";
            return 1;
        }
        return 0;
    }

    // Headless operations
    if (parser.hasOption("genome-query")) {
        genome::GenomeManager::initialize("config/genome_cache.json");
        genome::GenomeManager::requestGenomeData(parser.getOption("genome-query"));
        return 0;
    }

    if (parser.hasOption("discover-mesh")) {
        Graph graph;
        analytics::WorkerPool pool(4);
        analytics::MeshDiscoveryEngine engine(pool);
        engine.loadConfig("config/mesh_config.yaml");
        engine.loadMockData("config/mock_mesh_data.yaml");
        std::string seed = parser.getOption("discover-mesh");
        std::cout << "Discovering MeSH terms for: " << seed << "...\n";
        auto future = engine.runDiscovery(graph, seed);
        future.wait();
        std::cout << "Discovery complete. Nodes: " << graph.nodes.size() << "\n";

        if (parser.hasOption("save-graph")) {
            io::IOManager::saveGraphToCSV(graph, parser.getOption("save-graph"));
        }
        return 0;
    }

    if (parser.hasOption("load-graph") || parser.hasOption("save-graph") || parser.hasOption("get-node-details") || parser.hasOption("export-svg") || parser.hasOption("export-tui")) {
        Graph graph;
        std::string loadPath = parser.getOption("load-graph");
        if (loadPath.empty()) loadPath = "graph_input.csv";

        if (!io::IOManager::loadGraphFromCSV(graph, loadPath)) {
            std::cerr << "Error: Could not load graph from " << loadPath << "\n";
            return 1;
        }

        if (parser.hasOption("export-svg")) {
            ViewContext view;
            view.maxRenderDistance = 50; // Ensure all nodes in the graph are properly processed and layouted
            layout::LayoutManager::applyPerspectiveBFS(graph, view);
            std::string svgPath = parser.getOption("export-svg");
            if (io::IOManager::exportSVG(graph, svgPath)) {
                std::cout << "Graph exported to SVG: " << svgPath << "\n";
            } else {
                std::cerr << "Error: Could not export graph to " << svgPath << "\n";
                return 1;
            }
        }

        if (parser.hasOption("export-tui")) {
            std::string tuiPath = parser.getOption("export-tui");
            ViewContext view;
            view.width = 80;
            view.height = 25;
            view.currentViewMode = VM_PERSPECTIVE;
            view.zoomLevel = ZoomLevel::Z3;

            // Apply layout
            layout::LayoutManager::applyPerspectiveBFS(graph, view);

            // Render using ConsoleRenderer
            render::ConsoleRenderer renderer;
            renderer.initialize(view.width, view.height);

            SearchState search;
            input::ShortcutManager shortcut;
            renderer.renderWithUI(graph, view, search, shortcut);

            const render::FrameBuffer* fb = renderer.getFrameBuffer();
            if (fb) {
                std::ofstream out(tuiPath);
                if (out.is_open()) {
                    out << "<!DOCTYPE html>\n<html>\n<head>\n<style>\n"
                        << "body {\n"
                        << "  background-color: #1a1a1a;\n"
                        << "  display: flex;\n"
                        << "  justify-content: center;\n"
                        << "  align-items: center;\n"
                        << "  height: 100vh;\n"
                        << "  margin: 0;\n"
                        << "  font-family: sans-serif;\n"
                        << "}\n"
                        << ".terminal-window {\n"
                        << "  width: 720px;\n"
                        << "  background-color: #0c0c0c;\n"
                        << "  border-radius: 8px;\n"
                        << "  box-shadow: 0 20px 50px rgba(0,0,0,0.5);\n"
                        << "  border: 1px solid #333;\n"
                        << "  overflow: hidden;\n"
                        << "}\n"
                        << ".terminal-header {\n"
                        << "  background-color: #222;\n"
                        << "  padding: 10px;\n"
                        << "  display: flex;\n"
                        << "  align-items: center;\n"
                        << "  border-bottom: 1px solid #333;\n"
                        << "}\n"
                        << ".buttons {\n"
                        << "  display: flex;\n"
                        << "  gap: 6px;\n"
                        << "}\n"
                        << ".button {\n"
                        << "  width: 12px;\n"
                        << "  height: 12px;\n"
                        << "  border-radius: 50%;\n"
                        << "}\n"
                        << ".close { background-color: #ff5f56; }\n"
                        << ".minimize { background-color: #ffbd2e; }\n"
                        << ".maximize { background-color: #27c93f; }\n"
                        << ".title {\n"
                        << "  color: #999;\n"
                        << "  margin-left: auto;\n"
                        << "  margin-right: auto;\n"
                        << "  font-size: 13px;\n"
                        << "  font-weight: bold;\n"
                        << "}\n"
                        << ".terminal-body {\n"
                        << "  padding: 15px;\n"
                        << "}\n"
                        << "pre {\n"
                        << "  margin: 0;\n"
                        << "  font-family: 'Fira Code', 'Courier New', Courier, monospace;\n"
                        << "  font-size: 14px;\n"
                        << "  line-height: 1.25;\n"
                        << "  color: #00ff00;\n"
                        << "}\n"
                        << "</style>\n</head>\n<body>\n"
                        << "<div class=\"terminal-window\">\n"
                        << "  <div class=\"terminal-header\">\n"
                        << "    <div class=\"buttons\">\n"
                        << "      <div class=\"button close\"></div>\n"
                        << "      <div class=\"button minimize\"></div>\n"
                        << "      <div class=\"button maximize\"></div>\n"
                        << "    </div>\n"
                        << "    <div class=\"title\">" << fb->getTitle() << "</div>\n"
                        << "  </div>\n"
                        << "  <div class=\"terminal-body\">\n"
                        << "    <pre>";

                    for (const auto& row : fb->getBuffer()) {
                        // Escape HTML characters
                        for (char c : row) {
                            if (c == '<') out << "&lt;";
                            else if (c == '>') out << "&gt;";
                            else if (c == '&') out << "&amp;";
                            else out << c;
                        }
                        out << "\n";
                    }

                    out << "</pre>\n"
                        << "  </div>\n"
                        << "</div>\n"
                        << "</body>\n</html>\n";
                    std::cout << "Terminal GUI exported to HTML: " << tuiPath << "\n";
                } else {
                    std::cerr << "Error: Could not save TUI export to " << tuiPath << "\n";
                    return 1;
                }
            }
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
            if (io::IOManager::saveGraphToCSV(graph, savePath)) {
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