#ifndef OVERLAY_TYPES_H
#define OVERLAY_TYPES_H

#include <cstdint>

namespace model {

// Overlay match record for serialization
struct OverlayMatch {
    int32_t networkNodeId;
    int32_t overlayGraphId;
    int32_t overlayNodeId;
};

// Overlay edge endpoint for rendering off-screen connections
struct OverlayEdge {
    int32_t fromNodeId;
    int32_t toNodeId;
    float screenX;
    float screenY;
    bool isOffscreen;
};

} // namespace model

#endif // OVERLAY_TYPES_H
