#include "model_repository.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

namespace model {

static std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

bool ModelRepository::loadAtlas(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto tokens = split(line, ',');
        if (tokens.empty()) continue;

        if (tokens[0] == "REGION" && tokens.size() >= 7) {
            BrainRegion region;
            region.id = tokens[1];
            region.name = tokens[2];
            region.center = { std::stof(tokens[3]), std::stof(tokens[4]), std::stof(tokens[5]) };
            region.radius = std::stof(tokens[6]);
            model_.addRegion(region);
        } else if (tokens[0] == "PATHWAY" && tokens.size() >= 5) {
            BrainPathway pathway;
            pathway.id = tokens[1];
            pathway.name = tokens[2];
            pathway.sourceRegion = tokens[3];
            pathway.targetRegion = tokens[4];
            // Control points could follow
            for (size_t i = 5; i + 2 < tokens.size(); i += 3) {
                pathway.controlPoints.push_back({ std::stof(tokens[i]), std::stof(tokens[i+1]), std::stof(tokens[i+2]) });
            }
            model_.addPathway(pathway);
        }
    }
    return true;
}

bool ModelRepository::loadLabels(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto tokens = split(line, ',');
        if (tokens.empty()) continue;

        if (tokens[0] == "LABEL" && tokens.size() >= 5) {
            BrainLabel label;
            label.id = tokens[1];
            label.displayName = tokens[2];
            label.category = tokens[3];
            label.description = tokens[4];
            labels_.addLabel(label);
        }
    }
    return true;
}

bool ModelRepository::loadOverlay(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto tokens = split(line, ',');
        if (tokens.empty()) continue;

        if (tokens[0] == "MAP" && tokens.size() >= 3) {
            OverlayMapping mapping;
            mapping.graphNodeId = std::stoi(tokens[1]);
            mapping.regionId = tokens[2];
            if (tokens.size() >= 4) {
                mapping.pathwayId = tokens[3];
            }
            overlay_.addMapping(mapping);
        }
    }
    return true;
}

void ModelRepository::clearAll() {
    model_.clear();
    // Assuming labels and overlay also need clear methods if reusable
    // For now, they are small and re-loading overwrites or adds.
    // If needed, add clear() to BrainLabelSet and BrainOverlay.
}

} // namespace model
