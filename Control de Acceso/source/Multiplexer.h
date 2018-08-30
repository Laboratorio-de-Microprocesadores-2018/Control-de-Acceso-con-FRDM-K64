/* ____________________________________________________________________________
  /\               ITBA - Laboratorio de Microprocesadores - 2018              \  
  \_|                                 Grupo 3                                  |  
    |	Multiplexer.h                                                           |  
    |                                                                          |  
    |  ________________________________________________________________________|_ 
    \_/_________________________________________________________________________/
*/

#ifndef MULTIPLEXER_H_
#define MULTIPLEXER_H_

// Multiplexation frecuency
#define MUX_FRECUENCY 100

// Multiplexer initialization
void initMultiplexer();

// Returns the active pin in the multiplexer
int getActivePin();

#endif /* MULTIPLEXER_H_ */
