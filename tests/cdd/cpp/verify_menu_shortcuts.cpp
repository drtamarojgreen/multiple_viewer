#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include "ui/main_menu.h"
#include "input/shortcut_manager.h"
#include <sstream>

// Simple mock for std::cout to capture output
struct CoutRedirect {
    CoutRedirect(std::streambuf* new_buffer)
        : old(std::cout.rdbuf(new_buffer)) {}
    ~CoutRedirect() { std::cout.rdbuf(old); }
private:
    std::streambuf* old;
};

int main() {
    input::ShortcutManager sm;
    sm.registerShortcut('A', "Add Node", [](){});
    sm.registerShortcut('R', "Remove Node", [](){});
    sm.registerShortcut('S', "Save CSV", [](){});
    sm.registerShortcut('U', "Load CSV", [](){});

    std::stringstream buffer;
    {
        CoutRedirect redirect(buffer.rdbuf());
        ui::MainMenu::draw(sm);
    }

    std::string output = buffer.str();

    bool foundA = output.find("[A] Add Node") != std::string::npos;
    bool foundR = output.find("[R] Remove Node") != std::string::npos;
    bool foundS = output.find("[S] Save CSV") != std::string::npos;
    bool foundU = output.find("[U] Load CSV") != std::string::npos;

    if (foundA && foundR && foundS && foundU) {
        std::cout << "CDD Card PASSED: Menu correctly displays shortcuts." << std::endl;
        return 0;
    } else {
        std::cerr << "CDD Card FAILED: Menu missing shortcuts." << std::endl;
        if (!foundA) std::cerr << "  Missing [A]" << std::endl;
        if (!foundR) std::cerr << "  Missing [R]" << std::endl;
        if (!foundS) std::cerr << "  Missing [S]" << std::endl;
        if (!foundU) std::cerr << "  Missing [U]" << std::endl;
        return 1;
    }
}
