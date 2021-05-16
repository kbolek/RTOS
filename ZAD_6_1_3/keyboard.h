#ifndef KEYBOARD_H
#define KEYBOARD_H

enum KeyboardState {RELEASED,BUTTON_1,BUTTON_2,BUTTON_3,BUTTON_4};
void KeyboardInit(void);

void Keyboard_Thread(void *pvParameters);
enum KeyboardState eKeyboard_Read(void);

#endif
