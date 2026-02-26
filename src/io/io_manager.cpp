#include "io_manager.h"
#include <fstream>
#include <iostream>

namespace io {

bool IOManager::loadJSON(Graph& graph, const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;
    graph.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("\"label\":") != std::string::npos) {
            auto getVal = [&](const std::string& key) {
                size_t pos = line.find(key);
                if (pos == std::string::npos) return std::string("");
                size_t start = line.find(":", pos);
                if (start == std::string::npos) return std::string("");
                start++; // Skip colon
                while (start < line.size() && (std::isspace(line[start]) || line[start] == '\"')) start++;
                size_t end = start;
                while (end < line.size() && line[end] != '\"' && line[end] != ',' && line[end] != '}' && line[end] != ']') end++;
                // Trim trailing whitespace
                size_t actualEnd = end;
                while (actualEnd > start && std::isspace(line[actualEnd - 1])) actualEnd--;
                return line.substr(start, actualEnd - start);
            };
            std::string label = getVal("\"label\"");
            int index = std::stoi(getVal("\"index\""));
            graph.addNode(GraphNode(label, index));
        }
    }
    return true;
}

bool IOManager::saveJSON(const Graph& graph, const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) return false;
    file << "{\n  \"nodes\": [\n";
    for (size_t i = 0; i < graph.nodes.size(); ++i) {
        const auto& n = graph.nodes[i];
        file << "    {\"label\": \"" << n.label << "\", \"index\": " << n.index << "}";
        if (i < graph.nodes.size() - 1) file << ",";
        file << "\n";
    }
    file << "  ]\n}\n";
    return true;
}

bool IOManager::exportSVG(const Graph& graph, const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) return false;
    file << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"800\" height=\"600\">\n";
    file << "  <rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";
    file << "</svg>\n";
    return true;
}

} // namespace io
