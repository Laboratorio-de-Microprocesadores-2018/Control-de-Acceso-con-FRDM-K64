/* ____________________________________________________________________________
  /\               ITBA - Laboratorio de Microprocesadores - 2018              \
  \_|                                 Grupo 3                                  |  
    |	Keyboard.h                                                             |
    |                                                                          |  
    |  ________________________________________________________________________|_ 
    \_/_________________________________________________________________________/
*/
#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum{KB_KEY_UP,KB_KEY_DOWN,KB_NO_EVT}KeyboardbEventType;

typedef struct
{
	KeyboardbEventType type;
	char charCode;
	int row;
	int col;
}KeyboardEvent;

// Keyboard initialization
void initKeyboard();

// Services
KeyboardEvent getKeyboardEvent();
char getchar();
bool keyboardHit();
void keyboardFlush();
uint8_t keyboardCount();




#endif /* KEYBOARD_H_ */
