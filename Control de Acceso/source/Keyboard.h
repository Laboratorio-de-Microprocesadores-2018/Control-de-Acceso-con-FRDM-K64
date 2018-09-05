/////////////////////////////////////////////////////////////////////////////////
//                              CONTROL DE ACCESO                              //
//          Grupo 3 - Laboratorio de Microprocesadores - ITBA - 2018           //
//	                                                                           //
/////////////////////////////////////////////////////////////////////////////////

/**
 * @file     Keyboard.h
 * @brief    Numerical matrix keyboard driver
 * @author   Tobías Lifschitz
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

/////////////////////////////////////////////////////////////////////////////////
//                             Included header files                           //
/////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <stdbool.h>


#define DEBOUNCE_TIME 20
#define MULTI_PRESS_TIME 200
#define LONG_PRESS_TIME 9000
#define ACTIVE_LOW 0

/////////////////////////////////////////////////////////////////////////////////
//                    Enumerations, structures and typedefs                    //
/////////////////////////////////////////////////////////////////////////////////

typedef enum{	KB_KEY_UP,
				KB_KEY_DOWN,
				KB_KEY_LONG_PRESS,
				KB_KEY_SINGLE_PRESS,
				KB_KEY_MULTI_PRESS,
				KB_NO_EVT} KeyboardEventType;


typedef struct
{
	KeyboardEventType type;
	char charCode;
	uint8_t clicks;
}KeyboardEvent;

/////////////////////////////////////////////////////////////////////////////////
//                         Global function prototypes                          //
/////////////////////////////////////////////////////////////////////////////////

// Keyboard initialization
void initKeyboard();

// Services
KeyboardEvent getKeyboardEvent();
//char getchar();
bool keyboardHit();
void keyboardFlush();
uint8_t keyboardCount();


/////////////////////////////////////////////////////////////////////////////////
#endif /* KEYBOARD_H_ */
