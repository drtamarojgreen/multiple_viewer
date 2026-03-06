#include "io_manager.h"
#include <fstream>
#include <iostream>
#include <set>
#include <utility>
#include <algorithm>
#include <cctype>

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
        while (start < l.size() && (std::isspace(static_cast<unsigned char>(l[start])) || l[start] == '\"')) start++;
        size_t end = start;
        while (end < l.size() && l[end] != '\"' && l[end] != ',' && l[end] != '}' && l[end] != ']') end++;
        // Trim trailing whitespace
        size_t actualEnd = end;
        while (actualEnd > start && std::isspace(static_cast<unsigned char>(l[actualEnd - 1]))) actualEnd--;
        return l.substr(start, actualEnd - start);
    };

    while (std::getline(file, line)) {
        if (line.find("\"label\":") != std::string::npos) {
            std::string label = getVal(line, "\"label\"");
            std::string indexStr = getVal(line, "\"index\"");
            if (!indexStr.empty()) {
                try {
                    int index = std::stoi(indexStr);
                    graph.addNode(GraphNode(label, index));
                } catch (...) {}
            }
        } else if (line.find("\"source\":") != std::string::npos) {
            std::string srcStr = getVal(line, "\"source\"");
            std::string dstStr = getVal(line, "\"target\"");
            if (!srcStr.empty() && !dstStr.empty()) {
                try {
                    int src = std::stoi(srcStr);
                    int dst = std::stoi(dstStr);
                    graph.addEdge(src, dst);
                } catch (...) {}
            }
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
    file << "  ],\n  \"edges\": [\n";

    std::set<std::pair<int, int>> exported;
    bool firstEdge = true;
    for (const auto& node : graph.nodes) {
        for (int nbr : node.neighbors) {
            int u = node.index;
            int v = nbr;
            if (u > v) std::swap(u, v);
            if (exported.find({u, v}) == exported.end()) {
                if (!firstEdge) file << ",\n";
                file << "    {\"source\": " << u << ", \"target\": " << v << "}";
                exported.insert({u, v});
                firstEdge = false;
            }
        }
    }
    file << "\n  ]\n}\n";
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
