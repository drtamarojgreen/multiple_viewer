#ifndef CONSOLE_LOGIC_H
#define CONSOLE_LOGIC_H

void init_terminal();
void restore_terminal();
int get_char_non_blocking();

#endif // CONSOLE_LOGIC_H