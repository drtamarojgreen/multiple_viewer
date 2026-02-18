#include "file_logic.h"
#include "map_logic.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <cstddef>

bool saveGraphToCSV(const Graph& graph, const std::string& filename) {
    std::cout << "Starting save...\n";

    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << "\n";
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
    std::cout << "Graph saved to: " << filename << "\n";
    return true;
}

bool loadGraphFromCSV(Graph& graph, const std::string& filename) {
    auto start = std::chrono::high_resolution_clock::now();
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cerr << "Failed to open file for reading: " << filename << "\n";
        return false;
    }

    graph.clear();
    std::string line;
    size_t lineNo = 0;
    std::vector<std::vector<std::string>> neighborTokens;
    std::vector<int> loadedNodeIds;

    // First pass: parse each line into a node and neighbor token list
    while (std::getline(in, line)) {
        ++lineNo;

        if (line.empty()) continue;

        // 1) Extract quoted label
        size_t q1 = line.find('"');
        size_t q2 = (q1 == std::string::npos) ? std::string::npos : line.find('"', q1 + 1);
        if (q1 == std::string::npos || q2 == std::string::npos) {
            std::cerr << "[ERROR] Line " << lineNo << ": missing quoted label\n";
            in.close();
            return false;
        }
        std::string label = line.substr(q1 + 1, q2 - q1 - 1);

        // 2) Rest of line after closing quote comma
        size_t restPos = line.find(',', q2);
        if (restPos == std::string::npos) {
            continue;
        }
        std::string rest = line.substr(restPos + 1);

        std::vector<std::string> fields;
        {
            std::string cur;
            bool inQuotes = false;
            bool inBracket = false;

            for (size_t i = 0; i < line.size(); ++i) {
                char c = line[i];
                if (c == '"') {
                    inQuotes = !inQuotes;
                    cur += c;
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
            if (!cur.empty()) fields.push_back(cur);
        }
        if (fields.size() != 5) {
            std::cerr << "[ERROR] Line " << lineNo
                      << ": expected 5 fields (\"topicName\",index,[neighbors],weight,subjectIndex), got "
                      << fields.size() << "\n";
            in.close();
            return false;
        }
        GraphNode node;
        node.label = label;
        try {
            std::string nbrList = fields[2];
            if (!(nbrList.size() >= 2 && nbrList.front() == '[' && nbrList.back() == ']')) {
                std::cerr << "[WARN] Line " << lineNo << ": neighbor list missing brackets: '" << fields[2] << "'\n";
            } else {
                nbrList = nbrList.substr(1, nbrList.size() - 2);
            }

            std::vector<std::string> nbrStrs;
            std::stringstream ns(nbrList);
            std::string part;
            while (std::getline(ns, part, ',')) {
                if (!part.empty()) nbrStrs.push_back(part);
            }
            neighborTokens.push_back(std::move(nbrStrs));

            node.index        = std::stoi(fields[1]);
            node.neighbors    = {}; // Will be populated in second pass
            node.weight       = std::stoi(fields[3]);
            node.subjectIndex = std::stoi(fields[4]);
            loadedNodeIds.push_back(node.index);
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Line " << lineNo << ": parsing failed: " << e.what() << "\n";
            in.close();
            return false;
        }
        graph.addNode(node);
    }

    // Second pass: convert neighbor tokens to ints and add edges symmetrically
    for (size_t i = 0; i < loadedNodeIds.size(); ++i) {
        int from = loadedNodeIds[i];
        for (const auto& tok : neighborTokens[i]) {
            try {
                int to = std::stoi(tok);
                // To avoid duplicate edges in undirected graph loading,
                // only add if from < to OR if the other node hasn't been added yet
                if (graph.nodeExists(to)) {
                    bool alreadyExists = false;
                    for (int nbr : graph.nodeMap[from].neighbors) {
                        if (nbr == to) { alreadyExists = true; break; }
                    }
                    if (!alreadyExists) {
                        graph.addEdge(from, to);
                    }
                }
            } catch (...) {
                std::cerr << "[WARN] Failed to parse neighbor token: '" << tok << "' for node " << from << "\n";
            }
        }
    }

    in.close();
    auto end = std::chrono::high_resolution_clock::now();
    graph.summary.timeToLoadMs = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "Graph loaded in " << graph.summary.timeToLoadMs << " ms\n";
    return true;
}

std::set<int> parseNeighbors(const std::string& neighborStr) {
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

std::string serializeNodeToJson(const Graph& graph, int nodeId) {
    auto it = graph.nodeMap.find(nodeId);
    if (it == graph.nodeMap.end()) {
        return "{ \"error\": \"Node not found\" }";
    }

    const GraphNode& node = it->second;

    // Find position
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
    ss << "\"label\": \"" << node.label << "\","; // Note: This doesn't handle escaping quotes in the label
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