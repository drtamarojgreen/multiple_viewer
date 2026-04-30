#include "main_menu.h"
#include <iostream>
#include <iomanip>

namespace ui {

void MainMenu::draw(render::FrameBuffer& fb, int y, const input::ShortcutManager& shortcutManager) {
    fb.drawString(0, y++, "=== CBT Graph Viewer Menu ===", -1.0f);

    const auto& shortcuts = shortcutManager.getShortcuts();
    std::string line;
    for (size_t i = 0; i < shortcuts.size(); ++i) {
        line += "[" + std::string(1, shortcuts[i].key) + "] " + shortcuts[i].description + "  ";
        if ((i + 1) % 4 == 0 || i == shortcuts.size() - 1) {
            fb.drawString(0, y++, line, -1.0f);
            line.clear();
        }
    }

    fb.drawString(0, y++, "==============================", -1.0f);
}

} // namespace ui
