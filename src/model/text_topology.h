#ifndef TEXT_TOPOLOGY_H
#define TEXT_TOPOLOGY_H

#include <cstdint>
#include "model_common.h"
#include <string>

#include <vector>
#include <unordered_map>

namespace model {

enum class Direction {
    Anterior,
    Posterior,
    Superior,
    Inferior,
    Medial,
    Lateral,
    Unknown
};

struct AdjacencyEdge {
    RegionID targetRegion;
    Direction direction;
    float borderArea = 0.0f;
    float proximityScore = 1.0f;
};

struct RegionAdjacency {
    RegionID regionId;
    std::vector<AdjacencyEdge> neighbors;
};

struct RelationEdge {
    RegionID targetRegion;
    RelationType type;
};

struct RegionRelation {
    RegionID regionId;
    std::vector<RelationEdge> relations;
};

struct TextCell {
    char symbol = ' ';
    uint32_t color = 0xFFFFFFFF; // RGBA
    RegionID regionId;
    bool isBoundary = false;
};

struct TextCanvas {
    int width;
    int height;
    std::vector<TextCell> cells;

    TextCell& at(int x, int y) { return cells[y * width + x]; }
    const TextCell& at(int x, int y) const { return cells[y * width + x]; }
};

struct TextSlice {
    enum class Plane { Coronal, Sagittal, Axial };
    Plane plane;
    float position; // Coordinate value along the normal axis
    TextCanvas canvas;
    std::string summary;
};

} // namespace model

#endif // TEXT_TOPOLOGY_H
