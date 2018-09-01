/* ____________________________________________________________________________
  /\               ITBA - Laboratorio de Microprocesadores - 2018              \
  \_|                                 Grupo 3                                  |
    |	Display.h                                                              |
    |                                                                          |
    |  ________________________________________________________________________|_
    \_/_________________________________________________________________________/
*/
#ifndef DISPLAY_H_
#define DISPLAY_H_


// Display initialization
void initDisplay(void);


// Services
void putchar(char c);
void erase(void);
void dispClear(void);
void showCursor();
void blinkCursor();
void moveCursorLeft();
void moveCursorRight();
void showMessage(char * msg);

#endif /* DISPLAY_H_ */
