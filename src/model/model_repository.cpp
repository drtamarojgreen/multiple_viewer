#include "model_repository.h"
#include "../logger.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

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

ModelRepository::ModelRepository() {
    topology_ = createBrainTextTopology();
    indexer_ = createTopologyIndexer();
}

BrainTextTopology& ModelRepository::getTopology() { return *topology_; }
TopologyIndexer& ModelRepository::getIndexer() { return *indexer_; }

bool ModelRepository::loadAtlas(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        Logger::error("Failed to open atlas file: " + filepath);
        return false;
    }

    currentAtlasPath_ = filepath;
    model_.clear();

    std::string line;
    int lineNum = 0;
    while (std::getline(file, line)) {
        lineNum++;
        if (line.empty() || line[0] == '#') continue;
        auto tokens = split(line, ',');
        if (tokens.empty()) continue;

        if (tokens[0] == "REGION") {
            if (tokens.size() < 7) {
                Logger::warn("Malformed REGION at line " + std::to_string(lineNum) + ": insufficient fields.");
                continue;
            }

            try {
                BrainRegion region;
                region.id = tokens[1];
                region.name = tokens[2];
                region.center = { std::stof(tokens[3]), std::stof(tokens[4]), std::stof(tokens[5]) };
                region.radius = std::stof(tokens[6]);

                if (tokens.size() > 7) region.regionCode = tokens[7];
                if (tokens.size() > 8) region.hemisphere = static_cast<Hemisphere>(std::stoi(tokens[8]));
                if (tokens.size() > 9) region.lobe = static_cast<Lobe>(std::stoi(tokens[9]));
                if (tokens.size() > 10) region.parentId = tokens[10];

                model_.addRegion(region);
            } catch (const std::exception& e) {
                Logger::warn("Error parsing REGION at line " + std::to_string(lineNum) + ": " + std::string(e.what()));
            }
        } else if (tokens[0] == "PATHWAY") {
            if (tokens.size() < 5) {
                Logger::warn("Malformed PATHWAY at line " + std::to_string(lineNum) + ": insufficient fields.");
                continue;
            }

            try {
                BrainPathway pathway;
                pathway.id = tokens[1];
                pathway.name = tokens[2];
                pathway.sourceRegion = tokens[3];
                pathway.targetRegion = tokens[4];

                if (tokens.size() > 5) pathway.direction = static_cast<PathwayDirection>(std::stoi(tokens[5]));
                if (tokens.size() > 6) pathway.strength = std::stof(tokens[6]);
                if (tokens.size() > 7) pathway.type = static_cast<PathwayType>(std::stoi(tokens[7]));

                model_.addPathway(pathway);
            } catch (const std::exception& e) {
                Logger::warn("Error parsing PATHWAY at line " + std::to_string(lineNum) + ": " + std::string(e.what()));
            }
        }
    }

    indexer_->buildIndex(model_);
    Logger::info("Atlas loaded successfully: " + filepath);
    return true;
}

bool ModelRepository::reloadAtlas() {
    if (currentAtlasPath_.empty()) return false;
    Logger::info("Reloading atlas from: " + currentAtlasPath_);
    return loadAtlas(currentAtlasPath_);
}

bool ModelRepository::exportToJSON(const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) return false;

    file << "{\n  \"regions\": [\n";
    const auto& regions = model_.getRegions();
    size_t rCount = 0;
    for (const auto& [id, region] : regions) {
        file << "    {\n";
        file << "      \"id\": \"" << id << "\",\n";
        file << "      \"name\": \"" << region.name << "\",\n";
        file << "      \"center\": [" << region.center.x << ", " << region.center.y << ", " << region.center.z << "],\n";
        file << "      \"radius\": " << region.radius << ",\n";
        file << "      \"parent\": \"" << region.parentId << "\"\n";
        file << "    }" << (++rCount == regions.size() ? "" : ",") << "\n";
    }
    file << "  ],\n  \"pathways\": [\n";
    const auto& pathways = model_.getPathways();
    size_t pCount = 0;
    for (const auto& [id, pathway] : pathways) {
        file << "    {\n";
        file << "      \"id\": \"" << id << "\",\n";
        file << "      \"source\": \"" << pathway.sourceRegion << "\",\n";
        file << "      \"target\": \"" << pathway.targetRegion << "\",\n";
        file << "      \"strength\": " << pathway.strength << "\n";
        file << "    }" << (++pCount == pathways.size() ? "" : ",") << "\n";
    }
    file << "  ]\n}\n";

    Logger::info("Atlas exported to JSON: " + filepath);
    return true;
}

void ModelRepository::mirrorAtlas() {
    Logger::info("Generating structural symmetry mirroring (L -> R)...");

    std::vector<BrainRegion> mirroredRegions;
    for (const auto& [id, region] : model_.getRegions()) {
        if (region.hemisphere == Hemisphere::Left) {
            BrainRegion mirrored = region;
            mirrored.id = region.id + "_R";
            mirrored.name = "Right " + region.name;
            mirrored.center.x = -region.center.x;
            mirrored.hemisphere = Hemisphere::Right;
            mirroredRegions.push_back(mirrored);
        }
    }

    for (const auto& r : mirroredRegions) {
        model_.addRegion(r);
    }

    Logger::info("Mirroring complete. Total regions: " + std::to_string(model_.getRegions().size()));
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
            if (tokens.size() >= 5) {
                mapping.confidence = std::stof(tokens[4]);
            }
            overlay_.addMapping(mapping);
        }
    }
    return true;
}

int ModelRepository::getSubjectIndexForRegion(const RegionID& id) const {
    auto it = regionToSubjectMap_.find(id);
    return (it != regionToSubjectMap_.end()) ? it->second : -1;
}

void ModelRepository::setSubjectIndexForRegion(const RegionID& id, int subjectIndex) {
    regionToSubjectMap_[id] = subjectIndex;
}

void ModelRepository::clearAll() {
    model_.clear();
    regionToSubjectMap_.clear();
}


} // namespace model
