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

typedef enum{PASS,ADD,DEL,OPEN,ERR}Message;
// Display initialization
void initDisplay(void);


// Services
void dispPutchar(char c);
void dispErase(void);
void dispClear(void);
void brightUp(void);
void brightDown(void);
//void dispMessage(Message msg); // IMPLEMENTAR
//void dispError(int n);   // IMPLEMENTAR
void showCursor();
void blinkCursor();
void moveCursorLeft();
void moveCursorRight();

#endif /* DISPLAY_H_ */
