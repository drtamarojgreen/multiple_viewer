#include "processor_logic.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

namespace tp {

std::vector<std::string> splitBlocks(const std::string& text) {
    std::vector<std::string> blocks;
    size_t pos = 0, start = 0;
    while ((pos = text.find("\n\n", start)) != std::string::npos) {
        std::string blk = text.substr(start, pos - start);
        if (!blk.empty()) blocks.push_back(blk);
        start = pos + 2;
    }
    std::string tail = text.substr(start);
    if (!tail.empty()) blocks.push_back(tail);
    return blocks;
}

std::vector<std::string> splitLines(const std::string& block) {
    std::vector<std::string> lines;
    size_t start = 0, pos;
    while ((pos = block.find_first_of("\r\n", start)) != std::string::npos) {
        if (pos > start) lines.push_back(block.substr(start, pos - start));
        start = pos + 1;
        while (start < block.size() && (block[start] == '\r' || block[start] == '\n')) ++start;
    }
    if (start < block.size()) lines.push_back(block.substr(start));
    return lines;
}

int processTopics(const std::filesystem::path& inDir, const std::filesystem::path& outDir) {
    try {
        if (!std::filesystem::exists(inDir)) {
            std::cerr << "Error: Input directory does not exist: " << inDir << "\n";
            return 1;
        }
        if (!std::filesystem::exists(outDir)) {
            std::filesystem::create_directories(outDir);
        }

        std::vector<Node> nodes;
        std::unordered_map<std::string, int> nodeIndex;
        std::vector<std::string> oneLiners, paragraphs;
        int subjectId = 0;

        for (auto& file : std::filesystem::directory_iterator(inDir)) {
            if (file.path().extension() != ".txt") continue;

            std::ifstream in(file.path());
            std::stringstream buffer;
            buffer << in.rdbuf();
            std::string text = buffer.str();

            auto blocks = splitBlocks(text);
            for (auto& blk : blocks) {
                auto lines = splitLines(blk);
                int w = 10;
                int firstNodeIdx = -1;
                for (auto& L : lines) {
                    int dots = (int)std::count(L.begin(), L.end(), '.');
                    bool isPara = dots > 1;
                    bool isOne  = (dots == 1 && L.size() < 40);
                    bool isTopic = (dots == 0);

                    if (isPara) paragraphs.push_back(L);
                    else if (isOne) oneLiners.push_back(L);
                    else if (isTopic) {
                        auto it = nodeIndex.find(L);
                        if (it == nodeIndex.end()) {
                            int idx = (int)nodes.size();
                            nodes.push_back({L, {}, std::max(w, 1), subjectId});
                            nodeIndex[L] = idx;
                            if (firstNodeIdx < 0) firstNodeIdx = idx;
                        } else {
                            int idx = it->second;
                            nodes[idx].weight = std::max(nodes[idx].weight, w);
                        }

                        w = std::max(1, w - 1);

                        if (firstNodeIdx >= 0 && nodeIndex[L] != firstNodeIdx) {
                            int currIdx = nodeIndex[L];
                            nodes[firstNodeIdx].neigh.insert(currIdx);
                            nodes[currIdx].neigh.insert(firstNodeIdx);
                        }
                    }
                }
                subjectId++;
            }
        }

        // CSV Export
        std::ofstream csv(outDir / "output.csv");
        csv << "Name,Index,Neighbors,Weight,SubjectIndex\n";
        for (size_t i = 0; i < nodes.size(); ++i) {
            auto& n = nodes[i];
            csv << '"' << n.name << "\"," << i << ",[";
            bool first = true;
            for (auto nid : n.neigh) {
                if (!first) csv << ',';
                first = false;
                csv << nid;
            }
            csv << "]," << n.weight << ',' << n.subj << "\n";
        }

        auto writeList = [&](const std::vector<std::string>& list, const std::filesystem::path& fn) {
            std::ofstream f(outDir / fn);
            for (auto& line : list) f << line << "\n";
        };

        writeList(oneLiners, "onelines.txt");
        writeList(paragraphs, "paragraphs.txt");

        std::cout << "Processing complete. Output saved to " << outDir << "\n";
        return 0;
    } catch (std::exception& ex) {
        std::cerr << "ERROR: " << ex.what() << "\n";
        return 2;
    }
}

}
