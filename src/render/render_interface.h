#ifndef RENDER_INTERFACE_H
#define RENDER_INTERFACE_H

#include "../map_logic.h"

namespace render {

class IRenderer {
public:
    virtual ~IRenderer() = default;

    virtual bool initialize(int width, int height) = 0;
    virtual void clear() = 0;
    virtual void render(const Graph& graph, const ViewContext& view) = 0;
    virtual void present() = 0;
    virtual void shutdown() = 0;

    virtual bool isWindowOpen() const = 0;
};

} // namespace render

#endif // RENDER_INTERFACE_H
