#ifndef BRAIN_INTERFACE_H
#define BRAIN_INTERFACE_H

#include "brain_canonical.h"
#include <string>
#include <vector>

namespace model {

class IBrainModelService {
public:
    virtual ~IBrainModelService() = default;

    virtual std::vector<BrainNode> getNodes() const = 0;
    virtual std::vector<BrainEdge> getEdges() const = 0;

    virtual void addNode(const BrainNode& node) = 0;
    virtual void addEdge(const BrainEdge& edge) = 0;

    virtual std::vector<std::string> searchNodes(const std::string& query) = 0;
};

} // namespace model

#endif // BRAIN_INTERFACE_H
