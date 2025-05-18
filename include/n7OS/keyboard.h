#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

void init_keyboard(void);
void keyboard_interrupt_handler(void);
char kgetch(void);

#endif