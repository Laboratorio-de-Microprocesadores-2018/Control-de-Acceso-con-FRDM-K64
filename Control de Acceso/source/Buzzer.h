/////////////////////////////////////////////////////////////////////////////////
//                              CONTROL DE ACCESO                              //
//          Grupo 3 - Laboratorio de Microprocesadores - ITBA - 2018           //
//	                                                                           //
/////////////////////////////////////////////////////////////////////////////////
/**
 * @file     Buzzer.h
 * @brief    Buzzer driver to make simple bips.
 */

#ifndef BUZZER_H_
#define BUZZER_H_

/////////////////////////////////////////////////////////////////////////////////
//                       Constants and macro definitions                       //
/////////////////////////////////////////////////////////////////////////////////

#define BIP_TIME_MS 300
#define DOUBLE_BIP_TIME_MS 200
#define LONG_BIP_TIME_MS 700

/////////////////////////////////////////////////////////////////////////////////
//                         Global function prototypes                          //
/////////////////////////////////////////////////////////////////////////////////

/**
 *
 */
void initBuzzer();

/**
 *
 */
void bip();

/**
 *
 */
void doubleBip();

/**
 *
 */
void longBip();

#endif /* BUZZER_H_ */
