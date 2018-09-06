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

#define BIP_TIME_MS 200
#define DOUBLE_BIP_TIME_MS 200
#define LONG_BIP_TIME_MS 1000
typedef enum{HIGH_PITCH,LOW_PITCH}Tone;
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
void bip(Tone t);

/**
 *
 */
void doubleBip(Tone t);

/**
 *
 */
void longBip(Tone t);

#endif /* BUZZER_H_ */
