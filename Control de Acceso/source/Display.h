/////////////////////////////////////////////////////////////////////////////////
//                              CONTROL DE ACCESO                              //
//          Grupo 3 - Laboratorio de Microprocesadores - ITBA - 2018           //
//	                                                                           //
/////////////////////////////////////////////////////////////////////////////////

/**
 * @file Display.h
 * @brief File containig services provided by the display driver.
 *
 *
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

/////////////////////////////////////////////////////////////////////////////////
//                    Enumerations, structures and typedefs                    //
/////////////////////////////////////////////////////////////////////////////////

/** Predefined text messages the display can show*/
typedef enum{SUDO,PASS,ADD,EDIT,DEL,OPEN,ERR}Message;

/**
 * @brief
 * @param
 */
void initDisplay(void);

/**
 * @brief
 * @param
 */
void dispPutchar(char c);

/**
 * @brief
 * @param
 */
void dispErase(void);


/**
 * @brief
 * @param
 */
void dispClear(void);


/**
 * @brief
 * @param
 */
void dispMessage(Message msg);
/**
 * @brief
 * @param
 */
void dispString(char * s); // PROBARRRRRRRRRRR

/**
 * @brief
 * @param
 */
void dispError(char c);


/**
 * @brief
 * @param
 */
void displayNum(int n);


/**
 * @brief
 * @param
 */
void brightnessUp(void);


/**
 * @brief
 * @param
 */
void brightnessDown(void);


/**
 * @brief
 * @param
 */
int getBrightnessLevel(void);


/**
 * @brief
 * @param
 */
//void showCursor();


/**
 * @brief
 * @param
 */
//void blinkCursor();


/**
 * @brief
 * @param
 */
//void moveCursorLeft();


/**
 * @brief
 * @param
 */
//void moveCursorRight();

#endif /* DISPLAY_H_ */
