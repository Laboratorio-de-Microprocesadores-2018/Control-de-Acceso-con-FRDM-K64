/////////////////////////////////////////////////////////////////////////////////
//                              CONTROL DE ACCESO                              //
//          Grupo 3 - Laboratorio de Microprocesadores - ITBA - 2018           //
//	                                                                           //
/////////////////////////////////////////////////////////////////////////////////


#ifndef DISPLAY_H_
#define DISPLAY_H_


// Display initialization
void initDisplay(void);


// Services
void putchar(char c);
void erase(void);
void dispClear(void);
void brightUp(void);
void brightDown(void);

void showCursor();
void blinkCursor();
void moveCursorLeft();
void moveCursorRight();
void showMessage(char * msg);

#endif /* DISPLAY_H_ */
