#include "main_menu.h"
#include <iostream>
#include <iomanip>

namespace ui {

void MainMenu::draw(const input::ShortcutManager& shortcutManager) {
    std::cout << "\n=== CBT Graph Viewer Menu ===\n";

    const auto& shortcuts = shortcutManager.getShortcuts();
    for (size_t i = 0; i < shortcuts.size(); ++i) {
        std::cout << "[" << shortcuts[i].key << "] " << shortcuts[i].description;
        if ((i + 1) % 4 == 0 || i == shortcuts.size() - 1) {
            std::cout << "\n";
        } else {
            std::cout << "  ";
        }
    }

    std::cout << "==============================\n";
}

} // namespace ui
