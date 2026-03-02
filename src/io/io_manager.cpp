#include "io_manager.h"
#include <fstream>
#include <iostream>

namespace io {

bool IOManager::loadJSON(Graph& graph, const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;
    graph.clear();
    std::string line;

    auto getVal = [&](const std::string& l, const std::string& key) {
        size_t pos = l.find(key);
        if (pos == std::string::npos) return std::string("");
        size_t start = l.find(":", pos);
        if (start == std::string::npos) return std::string("");
        start++; // Skip colon
        while (start < l.size() && (std::isspace(l[start]) || l[start] == '\"')) start++;
        size_t end = start;
        while (end < l.size() && l[end] != '\"' && l[end] != ',' && l[end] != '}' && l[end] != ']') end++;
        // Trim trailing whitespace
        size_t actualEnd = end;
        while (actualEnd > start && std::isspace(l[actualEnd - 1])) actualEnd--;
        return l.substr(start, actualEnd - start);
    };

    while (std::getline(file, line)) {
        if (line.find("\"label\":") != std::string::npos) {
            std::string label = getVal(line, "\"label\"");
            int index = std::stoi(getVal(line, "\"index\""));
            graph.addNode(GraphNode(label, index));
        } else if (line.find("\"source\":") != std::string::npos) {
            int src = std::stoi(getVal(line, "\"source\""));
            int dst = std::stoi(getVal(line, "\"target\""));
            graph.addEdge(src, dst);
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
