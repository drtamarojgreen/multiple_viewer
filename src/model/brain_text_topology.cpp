#include "brain_text_topology.h"
#include <cmath>
#include <algorithm>
#include <sstream>
#include <map>

namespace model {

class TopologyIndexerImpl : public TopologyIndexer {
public:
    void buildIndex(const BrainModel& model) override {
        // Simple implementation: precompute adjacency based on bounding sphere overlap
        // In a real system, this would use voxel geometry traversal
        adjacencyMap_.clear();
        const auto& regions = model.getRegions();

        for (auto it1 = regions.begin(); it1 != regions.end(); ++it1) {
            RegionAdjacency adjacency;
            adjacency.regionId = it1->first;

            for (auto it2 = regions.begin(); it2 != regions.end(); ++it2) {
                if (it1 == it2) continue;

                const auto& r1 = it1->second;
                const auto& r2 = it2->second;

                // Euclidean distance between centers
                float dx = r1.center.x - r2.center.x;
                float dy = r1.center.y - r2.center.y;
                float dz = r1.center.z - r2.center.z;
                float dist = std::sqrt(dx*dx + dy*dy + dz*dz);

                // If bounding spheres overlap slightly, consider them adjacent for text maps
                if (dist < (r1.radius + r2.radius) * 1.2f) {
                    AdjacencyEdge edge;
                    edge.targetRegion = r2.id;
                    edge.proximityScore = 1.0f / (dist + 0.1f);

                    // Determine direction
                    if (std::abs(dx) > std::abs(dy) && std::abs(dx) > std::abs(dz)) {
                        edge.direction = (dx > 0) ? Direction::Medial : Direction::Lateral; // Simplified
                    } else if (std::abs(dy) > std::abs(dx) && std::abs(dy) > std::abs(dz)) {
                        edge.direction = (dy > 0) ? Direction::Superior : Direction::Inferior;
                    } else {
                        edge.direction = (dz > 0) ? Direction::Anterior : Direction::Posterior;
                    }

                    adjacency.neighbors.push_back(edge);
                }
            }
            adjacencyMap_[it1->first] = adjacency;
        }
    }

    RegionID findNearestRegion(const Vec3& point) const override {
        // This would use a spatial tree in a real implementation
        return ""; // Stub
    }

private:
    std::unordered_map<RegionID, RegionAdjacency> adjacencyMap_;
};

class SimpleLabelPlacementPolicy : public LabelPlacementPolicy {
public:
    void placeLabels(const BrainModel& model, TextCanvas& canvas) override {
        // Track occupied cells to avoid collisions
        std::vector<bool> occupied(canvas.cells.size(), false);

        // Collect all unique regions in the canvas
        std::map<RegionID, std::vector<std::pair<int, int>>> regionPixels;
        for (int y = 0; y < canvas.height; ++y) {
            for (int x = 0; x < canvas.width; ++x) {
                const auto& cell = canvas.at(x, y);
                if (!cell.regionId.empty()) {
                    regionPixels[cell.regionId].push_back({x, y});
                }
            }
        }

        // Sort regions by display priority (descending)
        std::vector<const BrainRegion*> regionsToLabel;
        for (const auto& [id, pixels] : regionPixels) {
            const auto* region = model.getRegion(id);
            if (region) regionsToLabel.push_back(region);
        }

        std::sort(regionsToLabel.begin(), regionsToLabel.end(), [](const BrainRegion* a, const BrainRegion* b) {
            return a->displayPriority > b->displayPriority;
        });

        // Try to place labels
        for (const auto* region : regionsToLabel) {
            const auto& pixels = regionPixels[region->id];
            if (pixels.empty()) continue;

            // Calculate centroid as anchor
            int sumX = 0, sumY = 0;
            for (const auto& p : pixels) { sumX += p.first; sumY += p.second; }
            int cx = sumX / pixels.size();
            int cy = sumY / pixels.size();

            std::string labelText = region->regionCode;
            if (labelText.empty()) labelText = region->id.substr(0, 3);

            // Check if we can place the label starting at (cx, cy)
            bool canPlace = true;
            for (int i = 0; i < (int)labelText.length(); ++i) {
                int nx = cx + i;
                if (nx >= canvas.width || occupied[cy * canvas.width + nx]) {
                    canPlace = false;
                    break;
                }
            }

            if (canPlace) {
                for (int i = 0; i < (int)labelText.length(); ++i) {
                    auto& cell = canvas.at(cx + i, cy);
                    cell.symbol = labelText[i];
                    occupied[cy * canvas.width + (cx + i)] = true;
                }
            }
        }
    }
};

class BrainTextTopologyImpl : public BrainTextTopology {
public:
    TextSlice generateSlice(const BrainModel& model, TextSlice::Plane plane, float position, int resolution) override {
        TextSlice slice;
        slice.plane = plane;
        slice.position = position;

        slice.canvas.width = resolution;
        slice.canvas.height = resolution;
        slice.canvas.cells.resize(resolution * resolution);

        float range = 200.0f;
        float halfRange = range / 2.0f;

        std::set<RegionID> regionsInSlice;
        for (int y = 0; y < resolution; ++y) {
            for (int x = 0; x < resolution; ++x) {
                float px, py, pz;
                float fx = (static_cast<float>(x) / resolution) * range - halfRange;
                float fy = (static_cast<float>(y) / resolution) * range - halfRange;

                if (plane == TextSlice::Plane::Axial) {
                    px = fx; py = fy; pz = position;
                } else if (plane == TextSlice::Plane::Coronal) {
                    px = fx; py = position; pz = fy;
                } else { // Sagittal
                    px = position; py = fx; pz = fy;
                }

                RegionID regionId = model.findRegionAt({px, py, pz});
                auto& cell = slice.canvas.at(x, y);
                cell.regionId = regionId;
                cell.symbol = regionId.empty() ? '.' : '#';
                if (!regionId.empty()) regionsInSlice.insert(regionId);
            }
        }

        SimpleLabelPlacementPolicy policy;
        policy.placeLabels(model, slice.canvas);

        std::stringstream ss;
        ss << "Slice Summary: Plane=" << (int)plane << " Pos=" << position
           << " Regions=" << regionsInSlice.size();
        slice.summary = ss.str();

        return slice;
    }

    std::vector<RegionID> getNeighborhood(const BrainModel& model, const RegionID& id) override {
        std::vector<RegionID> neighbors;
        TopologyIndexerImpl indexer;
        indexer.buildIndex(model);

        // Find adjacency for this region
        // Since we don't expose adjacencyMap_ in the interface, we'd normally use a internal method
        // For this demo, we'll just re-calculate it or use the indexer if it had a query method
        const auto& regions = model.getRegions();
        auto it = regions.find(id);
        if (it == regions.end()) return neighbors;

        const auto& r1 = it->second;
        for (const auto& [nid, r2] : regions) {
            if (id == nid) continue;
            float dx = r1.center.x - r2.center.x;
            float dy = r1.center.y - r2.center.y;
            float dz = r1.center.z - r2.center.z;
            float distSq = dx*dx + dy*dy + dz*dz;
            if (distSq < (r1.radius + r2.radius) * (r1.radius + r2.radius) * 1.44f) {
                neighbors.push_back(nid);
            }
        }
        return neighbors;
    }

    std::string generateHierarchyTree(const BrainModel& model) override {
        std::stringstream ss;
        ss << "# Brain Region Hierarchy\n\n";

        std::map<RegionID, std::vector<RegionID>> children;
        std::vector<RegionID> roots;

        for (const auto& [id, region] : model.getRegions()) {
            if (region.parentId.empty()) {
                roots.push_back(id);
            } else {
                children[region.parentId].push_back(id);
            }
        }

        for (const auto& root : roots) {
            printNode(ss, root, children, 0, model);
        }

        return ss.str();
    }

    std::string generatePathwaySummary(const BrainModel& model) override {
        std::stringstream ss;
        ss << "### Pathway Connectivity Report\n\n";
        ss << "| Pathway | Source | Target | Type | Strength |\n";
        ss << "| :--- | :--- | :--- | :--- | :--- |\n";
        for (const auto& [id, pathway] : model.getPathways()) {
            ss << "| " << id << " | " << pathway.sourceRegion << " | " << pathway.targetRegion
               << " | " << (int)pathway.type << " | " << pathway.strength << " |\n";
        }
        return ss.str();
    }

    std::string exportToMarkdown(const TextSlice& slice) override {
        std::stringstream ss;
        ss << "### Brain Slice: " << slice.summary << "\n\n";
        ss << "```text\n";
        for (int y = 0; y < slice.canvas.height; ++y) {
            for (int x = 0; x < slice.canvas.width; ++x) {
                ss << slice.canvas.at(x, y).symbol;
            }
            ss << "\n";
        }
        ss << "```\n";
        return ss.str();
    }

    std::string exportToJSON(const TextSlice& slice) override {
        std::stringstream ss;
        ss << "{\n";
        ss << "  \"summary\": \"" << slice.summary << "\",\n";
        ss << "  \"plane\": " << (int)slice.plane << ",\n";
        ss << "  \"position\": " << slice.position << ",\n";
        ss << "  \"width\": " << slice.canvas.width << ",\n";
        ss << "  \"height\": " << slice.canvas.height << ",\n";
        ss << "  \"cells\": [\n";
        for (int i = 0; i < (int)slice.canvas.cells.size(); ++i) {
            const auto& cell = slice.canvas.cells[i];
            ss << "    {\"x\": " << (i % slice.canvas.width) << ", \"y\": " << (i / slice.canvas.width)
               << ", \"symbol\": \"" << cell.symbol << "\", \"regionId\": \"" << cell.regionId << "\"}";
            if (i < (int)slice.canvas.cells.size() - 1) ss << ",";
            ss << "\n";
        }
        ss << "  ]\n";
        ss << "}\n";
        return ss.str();
    }

    std::string exportToPlainText(const TextSlice& slice) override {
        std::stringstream ss;
        for (int y = 0; y < slice.canvas.height; ++y) {
            for (int x = 0; x < slice.canvas.width; ++x) {
                ss << slice.canvas.at(x, y).symbol;
            }
            ss << "\n";
        }
        return ss.str();
    }

private:


    void printNode(std::stringstream& ss, const RegionID& id, std::map<RegionID, std::vector<RegionID>>& children, int indent, const BrainModel& model) {
        for (int i = 0; i < indent; ++i) ss << "  ";
        const auto* region = model.getRegion(id);
        ss << "- " << (region ? region->name : id) << " (`" << id << "`)\n";

        auto it = children.find(id);
        if (it != children.end()) {
            for (const auto& childId : it->second) {
                printNode(ss, childId, children, indent + 1, model);
            }
        }
    }
};



// Factory functions (to be added to brain_text_topology.h)

std::unique_ptr<TopologyIndexer> createTopologyIndexer() {
    return std::make_unique<TopologyIndexerImpl>();
}

std::unique_ptr<BrainTextTopology> createBrainTextTopology() {
    return std::make_unique<BrainTextTopologyImpl>();
}

} // namespace model
