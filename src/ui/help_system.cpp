#include "help_system.h"

namespace ui {

std::string HelpSystem::getHelp(const std::string& context) {
    if (context == "Layout") return "Use the Layout menu to organize nodes via various algorithms.";
    return "Press 'H' for general help.";
}

void HelpSystem::showTutorial() {
    // Tutorial logic
}

} // namespace ui
