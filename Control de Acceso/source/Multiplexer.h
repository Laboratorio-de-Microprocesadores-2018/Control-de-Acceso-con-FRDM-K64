/////////////////////////////////////////////////////////////////////////////////
//                              CONTROL DE ACCESO                              //
//          Grupo 3 - Laboratorio de Microprocesadores - ITBA - 2018           //
//	                                                                           //
/////////////////////////////////////////////////////////////////////////////////

/**
 * @file     Multiplexer.h
 * @brief    Multiplexation abstraction for keyboard and display independence.
 * @author   Tobías Lifschitz
 */

#ifndef MULTIPLEXER_H_
#define MULTIPLEXER_H_

/////////////////////////////////////////////////////////////////////////////////
//                       Constants and macro definitions                       //
/////////////////////////////////////////////////////////////////////////////////
#define MUX_FREQUENCY 200

/**
 * @brief Init the multiplexer at the defined frequency
 */
void initMultiplexer();

/**
 * @brief Check the active pin in the multiplexer.
 * @return The active pin number between 0 and 3.
 */
int getActivePin();


/////////////////////////////////////////////////////////////////////////////////
#endif /* MULTIPLEXER_H_ */
