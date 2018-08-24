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
// Interrupt subroutine called to multiplex the display
void displayPISR(void);

// Services
void putchar(char c);
void dispClear();
void showCursor();
void blinkCursor();
void moveCursorLeft();
void moveCursorRight();
void showMessage(char * msg);

#endif /* DISPLAY_H_ */
