#include "status_bar.h"
#include <string>

namespace ui {

void StatusBar::draw(render::FrameBuffer& fb, int y, const SearchState& search) {
    std::string text;
    if (search.isActive) {
        text = " SEARCH: " + search.keyword + "_";
        if (search.lastSearchHadNoResults) {
            text += " [No Results]";
        } else if (!search.matches.empty()) {
            text += " [" + std::to_string(search.activeMatchIndex + 1) + "/" + std::to_string(search.matches.size()) + "]";
        }
    } else {
        text = " [Mode: Navigation]";
    }
    fb.drawString(0, y, text, -1.0f);
}

} // namespace ui
