#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include "../search_logic.h"
#include "../render/frame_buffer.h"

namespace ui {

class StatusBar {
public:
    static void draw(render::FrameBuffer& fb, int y, const SearchState& search);
};

} // namespace ui

#endif // STATUS_BAR_H
