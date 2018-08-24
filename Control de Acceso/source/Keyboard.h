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

typedef int KbEvtType;
static const KbEvtType KB_KEY_UP = 0;
static const KbEvtType KB_KEY_DOWN = 1;

typedef struct{
	char c;
	int row;
	int col;
	KbEvtType type;
}KbEvt;


// Keyboard initialization
void initKeyboard(void);

// Interrupt subroutine called when a button is pressed
void keyboardISR(void);

// Interrupt subroutine called to multiplex the keyboard
void keyboardPISR(void);

// Services
bool kbhit(void);
char getchar(void);
void kbClear(void);
char kbPeek(void);
uint8_t kbCount(void);



#endif /* KEYBOARD_H_ */
