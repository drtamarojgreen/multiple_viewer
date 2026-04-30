#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "../input/shortcut_manager.h"

namespace ui {

class MainMenu {
public:
    static void draw(const input::ShortcutManager& shortcutManager);
};

} // namespace ui

#endif // MAIN_MENU_H
