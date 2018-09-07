/////////////////////////////////////////////////////////////////////////////////
//                              CONTROL DE ACCESO                              //
//          Grupo 3 - Laboratorio de Microprocesadores - ITBA - 2018           //
//	                                                                           //
/////////////////////////////////////////////////////////////////////////////////
/**
 * @file     Buzzer.h
 * @brief    File containing simple services to make bips with piezzo buzzer.
 */

#ifndef BUZZER_H_
#define BUZZER_H_

/////////////////////////////////////////////////////////////////////////////////
//                       Constants and macro definitions                       //
/////////////////////////////////////////////////////////////////////////////////

/** Time duration of sounds*/
#define BIP_TIME_MS 200
#define DOUBLE_BIP_TIME_MS 200
#define LONG_BIP_TIME_MS 1000

/** Tone for the HIGH_PITCH sound*/
#define BUZZER_FREQ 2500

/////////////////////////////////////////////////////////////////////////////////
//                    Enumerations, structures and typedefs                    //
/////////////////////////////////////////////////////////////////////////////////

/** Desired tone for the sound. LOW is the half frequency of HIGH. */
typedef enum{HIGH_PITCH,LOW_PITCH}Tone;

/////////////////////////////////////////////////////////////////////////////////
//                         Global function prototypes                          //
/////////////////////////////////////////////////////////////////////////////////

/**
 *	@brief Initializate buzzer.
 */
void initBuzzer();

/**
 * @brief Produce a short bip.
 * @param t Desired tone, LOW_PITCH or HIGH_PITCH
 */
void bip(Tone t);

/**
 * @brief Produce a double bip.
 * @param t Desired tone, LOW_PITCH or HIGH_PITCH
 */
void doubleBip(Tone t);

/**
 * @brief Produce a long bip.
 * @param t Desired tone, LOW_PITCH or HIGH_PITCH
 */
void longBip(Tone t);

#endif /* BUZZER_H_ */
