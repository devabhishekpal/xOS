#ifndef KEYBOARD_HEADER
#define KEYBOARD_HEADER

void init_keyboard(void);
void keyboard_test(void);
char get_scancode(void);
char get_character(unsigned int scancode);

#endif