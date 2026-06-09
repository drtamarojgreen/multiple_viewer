#include "io_manager.h"
#include "../logger.h"
#include <fstream>
#include <iostream>
#include <set>
#include <utility>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <chrono>
#include <mutex>

namespace io {

static StorageBackend* g_backend = nullptr;
static std::mutex g_backend_mutex;

void IOManager::setBackend(StorageBackend* backend) {
    std::lock_guard<std::mutex> lock(g_backend_mutex);
    g_backend = backend;
}

static std::string getJsonVal(const std::string& l, const std::string& key) {
    size_t pos = l.find(key);
    if (pos == std::string::npos) return std::string("");

    // Find the colon after the key
    size_t colon = l.find(":", pos + key.length());
    if (colon == std::string::npos) return std::string("");

    size_t start = colon + 1;
    // Skip spaces and opening quote
    while (start < l.size() && (std::isspace(static_cast<unsigned char>(l[start])) || l[start] == '\"')) start++;

    size_t end = start;
    // Values end at a quote, comma, closing brace or bracket
    while (end < l.size() && l[end] != '\"' && l[end] != ',' && l[end] != '}' && l[end] != ']') end++;

    // Trim trailing whitespace
    size_t actualEnd = end;
    while (actualEnd > start && std::isspace(static_cast<unsigned char>(l[actualEnd - 1]))) actualEnd--;

    return l.substr(start, actualEnd - start);
}

bool LocalFS::read(const std::string& path, std::string& outData) {
    std::ifstream in(path, std::ios::in | std::ios::binary);
    if (!in) return false;
    std::ostringstream ss;
    ss << in.rdbuf();
    outData = ss.str();
    return true;
}

bool LocalFS::write(const std::string& path, const std::string& inData) {
    std::ofstream out(path, std::ios::out | std::ios::binary);
    if (!out) return false;
    out << inData;
    return true;
}

bool IOManager::loadJSON(Graph& graph, const std::string& filepath) {
    std::string data;
    {
        std::lock_guard<std::mutex> lock(g_backend_mutex);
        if (g_backend) {
            if (!g_backend->read(filepath, data)) {
                return false;
            }
        } else {
            std::ifstream file(filepath);
            if (!file.is_open()) return false;
            std::ostringstream ss;
            ss << file.rdbuf();
            data = ss.str();
        }
    }

    graph.clear();
    std::stringstream file(data);
    std::string line;

    while (std::getline(file, line)) {
        if (line.find("\"label\":") != std::string::npos) {
            std::string label = getJsonVal(line, "\"label\"");
            std::string indexStr = getJsonVal(line, "\"index\"");
            if (!indexStr.empty()) {
                try {
                    int index = std::stoi(indexStr);
                    graph.addNode(GraphNode(label, index));
                } catch (const std::exception& e) {
                    Logger::error("Failed to parse node index: " + indexStr + " Error: " + e.what());
                }
            }
        } else if (line.find("\"source\":") != std::string::npos) {
            std::string srcStr = getJsonVal(line, "\"source\"");
            std::string dstStr = getJsonVal(line, "\"target\"");
            if (!srcStr.empty() && !dstStr.empty()) {
                try {
                    int src = std::stoi(srcStr);
                    int dst = std::stoi(dstStr);
                    graph.addEdge(src, dst);
                } catch (const std::exception& e) {
                    Logger::error("Failed to parse edge: " + srcStr + "->" + dstStr + " Error: " + e.what());
                }
            }
        }
    }
    return true;
}

bool IOManager::loadMeshJSON(Graph& graph, const std::string& filepath) {
    std::string data;
    {
        std::lock_guard<std::mutex> lock(g_backend_mutex);
        if (g_backend) {
            if (!g_backend->read(filepath, data)) return false;
        } else {
            std::ifstream file(filepath);
            if (!file.is_open()) return false;
            std::ostringstream ss;
            ss << file.rdbuf();
            data = ss.str();
        }
    }

    graph.clear();
    std::stringstream file(data);
    std::string line;

    while (std::getline(file, line)) {
        if (line.find("\"label\":") != std::string::npos) {
            GraphNode node;
            node.label = getJsonVal(line, "\"label\"");
            std::string idStr = getJsonVal(line, "\"id\"");
            if (idStr.empty()) idStr = getJsonVal(line, "\"index\"");

            if (!idStr.empty()) {
                try {
                    node.index = std::stoi(idStr);
                    std::string weightStr = getJsonVal(line, "\"weight\"");
                    if (!weightStr.empty()) node.weight = std::stoi(weightStr);
                    std::string subjectStr = getJsonVal(line, "\"subjectIndex\"");
                    if (!subjectStr.empty()) node.subjectIndex = std::stoi(subjectStr);

                    graph.addNode(node);
                } catch (const std::exception& e) {
                    Logger::error("Failed to parse mesh node: " + idStr + " Error: " + e.what());
                }
            }
        } else if (line.find("\"source\":") != std::string::npos) {
            std::string srcStr = getJsonVal(line, "\"source\"");
            std::string dstStr = getJsonVal(line, "\"target\"");
            if (!srcStr.empty() && !dstStr.empty()) {
                try {
                    int src = std::stoi(srcStr);
                    int dst = std::stoi(dstStr);
                    graph.addEdge(src, dst);
                } catch (const std::exception& e) {
                    Logger::error("Failed to parse mesh edge: " + srcStr + "->" + dstStr + " Error: " + e.what());
                }
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
    file << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"1200\" height=\"800\">\n";
    file << "  <rect width=\"100%\" height=\"100%\" fill=\"#1e1e1e\"/>\n";

    // Find min/max for centering
    float minX = 1e9, maxX = -1e9, minY = 1e9, maxY = -1e9;
    for (const auto& [id, p] : graph.layoutPositions) {
        minX = std::min(minX, p.x); maxX = std::max(maxX, p.x);
        minY = std::min(minY, p.y); maxY = std::max(maxY, p.y);
    }

    float offsetX = 600 - (minX + maxX) * 25; // Increased scale for centering
    float offsetY = 400 - (minY + maxY) * 35;

    // Draw Edges
    for (const auto& node : graph.nodes) {
        if (graph.layoutPositions.count(node.index) == 0) continue;
        const auto& p1 = graph.layoutPositions.at(node.index);

        for (int neighbor_id : node.neighbors) {
            if (graph.layoutPositions.count(neighbor_id) == 0) continue;
            if (node.index > neighbor_id) continue; // Draw each edge once
            const auto& p2 = graph.layoutPositions.at(neighbor_id);

            file << "  <line x1=\"" << offsetX + p1.x * 50 << "\" y1=\"" << offsetY + p1.y * 70
                 << "\" x2=\"" << offsetX + p2.x * 50 << "\" y2=\"" << offsetY + p2.y * 70
                 << "\" stroke=\"#555\" stroke-width=\"2\" />\n";
        }
    }

    // Draw Nodes
    for (const auto& node : graph.nodes) {
        if (graph.layoutPositions.count(node.index) == 0) continue;
        const auto& p = graph.layoutPositions.at(node.index);

        std::string color = "#4a90e2"; // Default Blue
        if (node.weight >= 10) color = "#e74c3c"; // Red for high weight
        else if (node.weight >= 5) color = "#f1c40f"; // Yellow

        float radius = 10.0f + (node.weight * 0.5f); // Scale radius by weight

        file << "  <circle cx=\"" << offsetX + p.x * 50 << "\" cy=\"" << offsetY + p.y * 70
             << "\" r=\"" << radius << "\" fill=\"" << color << "\" stroke=\"white\" stroke-width=\"1\" />\n";
        file << "  <text x=\"" << offsetX + p.x * 50 << "\" y=\"" << offsetY + p.y * 70 + radius + 15
             << "\" fill=\"white\" font-family=\"Arial\" font-size=\"12\" font-weight=\"bold\" text-anchor=\"middle\">" << node.label << "</text>\n";
    }

    file << "</svg>\n";
    return true;
}

bool IOManager::saveGraphToCSV(const Graph& graph, const std::string& filename) {
    Logger::info("Starting graph save to " + filename);

    std::ofstream out(filename);
    if (!out.is_open()) {
        Logger::error("Failed to open file for writing: " + filename);
        return false;
    }


    // Format: "Label",Index,[Neighbors],Weight,SubjectIndex
    for (const auto& node : graph.nodes) {
        out << "\"" << node.label << "\"," << node.index << ",[";
        for (size_t i = 0; i < node.neighbors.size(); ++i) {
            out << node.neighbors[i];
            if (i + 1 < node.neighbors.size()) out << ",";
        }
        out << "]," << node.weight << "," << node.subjectIndex << "\n";
    }
    out.close();
    Logger::info("Graph successfully saved to: " + filename);
    return true;
}

bool IOManager::loadGraphFromCSV(Graph& graph, const std::string& filename) {
    auto start = std::chrono::high_resolution_clock::now();
    Logger::info("Loading graph from " + filename);
    std::ifstream in(filename);
    if (!in.is_open()) {
        Logger::error("Failed to open file for reading: " + filename);
        return false;
    }

    graph.clear();
    std::string line;
    size_t lineNo = 0;
    std::vector<std::vector<std::string>> neighborTokens;
    std::vector<int> loadedNodeIds;

    while (std::getline(in, line)) {
        ++lineNo;
        if (line.empty()) continue;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        std::vector<std::string> fields;
        {
            std::string cur;
            bool inQuotes = false;
            bool inBracket = false;

            for (size_t i = 0; i < line.size(); ++i) {
                char c = line[i];
                if (c == '"') {
                    inQuotes = !inQuotes;
                } else if (c == '[') {
                    inBracket = true;
                    cur += c;
                } else if (c == ']') {
                    inBracket = false;
                    cur += c;
                } else if (c == ',' && !inQuotes && !inBracket) {
                    fields.push_back(cur);
                    cur.clear();
                } else {
                    cur += c;
                }
            }
            fields.push_back(cur);
        }

        if (fields.size() < 5) {
            std::cerr << "[ERROR] Line " << lineNo << ": expected at least 5 fields, got " << fields.size() << ": " << line << "\n";
            in.close();
            return false;
        }

        GraphNode node;
        node.label = fields[0];
        try {
            node.index        = std::stoi(fields[1]);

            std::string nbrList = fields[2];
            if (!nbrList.empty() && nbrList.front() == '[' && nbrList.back() == ']') {
                nbrList = nbrList.substr(1, nbrList.size() - 2);
            }

            std::vector<std::string> nbrStrs;
            std::stringstream ns(nbrList);
            std::string part;
            while (std::getline(ns, part, ',')) {
                if (!part.empty()) nbrStrs.push_back(part);
            }
            neighborTokens.push_back(std::move(nbrStrs));

            node.weight       = std::stoi(fields[3]);
            node.subjectIndex = std::stoi(fields[4]);
            loadedNodeIds.push_back(node.index);
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Line " << lineNo << ": parsing failed: " << e.what() << "\n";
            continue;
        }
        graph.addNode(node);
    }

    in.close();

    for (size_t i = 0; i < loadedNodeIds.size(); ++i) {
        if (i >= neighborTokens.size()) break;
        int from = loadedNodeIds[i];
        if (!graph.nodeExists(from)) continue;

        for (const auto& tok : neighborTokens[i]) {
            try {
                if (tok.empty()) continue;
                int to = std::stoi(tok);
                if (graph.nodeExists(to) && from != to) {
                    bool alreadyExists = false;
                    const auto& fromNode = graph.nodeMap.at(from);
                    for (int nbr : fromNode.neighbors) {
                        if (nbr == to) { alreadyExists = true; break; }
                    }
                    if (!alreadyExists) {
                        graph.addEdge(from, to);
                    }
                }
            } catch (const std::exception& e) {
                std::cerr << "[WARN] Failed to parse neighbor token: '" << tok << "' for node " << from << ": " << e.what() << "\n";
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    graph.summary.timeToLoadMs = std::chrono::duration<double, std::milli>(end - start).count();
    Logger::info("Graph loaded in " + std::to_string(graph.summary.timeToLoadMs) + " ms");
    return true;
}

std::set<int> IOManager::parseNeighbors(const std::string& neighborStr) {
    std::set<int> neighbors;
    if (neighborStr.empty() || neighborStr == "[]") return neighbors;

    std::string cleaned = neighborStr;
    if (cleaned.front() == '[') cleaned.erase(0, 1);
    if (cleaned.back() == ']') cleaned.pop_back();

    std::stringstream ss(cleaned);
    std::string token;
    int limit = 0;

    while (std::getline(ss, token, ',')) {
        if (++limit > 20) {
            std::cerr << "[WARN] parseNeighbors hit safety limit (20 entries)\n";
            break;
        }
        try {
            neighbors.insert(std::stoi(token));
        } catch (...) {
            std::cerr << "[WARN] parseNeighbors invalid token: " << token << "\n";
        }
    }

    return neighbors;
}

std::string IOManager::serializeNodeToJson(const Graph& graph, int nodeId) {
    auto it = graph.nodeMap.find(nodeId);
    if (it == graph.nodeMap.end()) {
        return "{ \"error\": \"Node not found\" }";
    }

    const GraphNode& node = it->second;

    auto pos_it = graph.nodePos.find(nodeId);
    int x = 0, y = 0, z = 0;
    if (pos_it != graph.nodePos.end()) {
        x = pos_it->second.x;
        y = pos_it->second.y;
        z = pos_it->second.z;
    }

    std::stringstream ss;
    ss << "{";
    ss << "\"id\": " << nodeId << ",";
    ss << "\"label\": \"" << node.label << "\",";
    ss << "\"position\": { \"x\": " << x << ", \"y\": " << y << ", \"z\": " << z << " },";
    ss << "\"weight\": " << node.weight << ",";
    ss << "\"subjectIndex\": " << node.subjectIndex << ",";
    ss << "\"degree\": " << node.neighbors.size() << ",";
    ss << "\"neighbors\": [";
    for (size_t i = 0; i < node.neighbors.size(); ++i) {
        ss << node.neighbors[i];
        if (i < node.neighbors.size() - 1) {
            ss << ",";
        }
    }
    ss << "]";
    ss << "}";

    return ss.str();
}

} // namespace io
