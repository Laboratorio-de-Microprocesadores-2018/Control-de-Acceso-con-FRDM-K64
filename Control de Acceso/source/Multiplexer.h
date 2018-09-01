/***************************************************************************//**
  @file     Multiplexer.h
  @brief    Multiplexation abstraction for keyboard and display independence.
  @author   Grupo 3 - Laboratorio de Microprocesadores - 2018
 ******************************************************************************/

#ifndef MULTIPLEXER_H_
#define MULTIPLEXER_H_

// Multiplexation frecuency
#define MUX_FREQUENCY 100

/**
 * @brief Init the multiplexer
 */
void initMultiplexer();

/**
 * @brief Check the active pin in the multiplexer.
 * @return The active pin number between 0 and 3.
 */
int getActivePin();

#endif /* MULTIPLEXER_H_ */
