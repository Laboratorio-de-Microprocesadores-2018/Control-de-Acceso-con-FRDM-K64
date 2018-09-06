/////////////////////////////////////////////////////////////////////////////////
//                              CONTROL DE ACCESO                              //
//          Grupo 3 - Laboratorio de Microprocesadores - ITBA - 2018           //
//	                                                                           //
/////////////////////////////////////////////////////////////////////////////////


#ifndef DISPLAY_H_
#define DISPLAY_H_

typedef enum{PASS,ADD,DEL,OPEN,ERR}Message;
// Display initialization
void initDisplay(void);


// Services
void dispPutchar(char c);
void dispErase(void);
void dispClear(void);
void dispMessage(Message msg);
void dispError(char c);
void displayNum(int n);
void brightUp(void);
void brightDown(void);


void showCursor();
void blinkCursor();
void moveCursorLeft();
void moveCursorRight();

#endif /* DISPLAY_H_ */
