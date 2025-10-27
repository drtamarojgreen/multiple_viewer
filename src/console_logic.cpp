#include "console_logic.h"

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#ifndef _WIN32
static struct termios oldt, newt;

void init_terminal() {
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

void restore_terminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, 0);
}

int get_char_non_blocking() {
    return getchar();
}

#else // For Windows
void init_terminal() {
    // No initialization needed for Windows
}

void restore_terminal() {
    // No restoration needed for Windows
}

int get_char_non_blocking() {
    if (_kbhit()) {
        return _getch();
    }
    return -1; // No key pressed
}
#endif