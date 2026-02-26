#ifndef HELP_SYSTEM_H
#define HELP_SYSTEM_H

#include <string>

namespace ui {

class HelpSystem {
public:
    static std::string getHelp(const std::string& context);
    static void showTutorial();
};

} // namespace ui

#endif // HELP_SYSTEM_H
