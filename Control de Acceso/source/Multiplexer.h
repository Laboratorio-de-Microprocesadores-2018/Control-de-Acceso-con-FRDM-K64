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


// Multiplexer initialization
void initMultiplexer(int freq);

// Interrupt subroutine called to switch multiplexer
void multiplexerPISR(void);

// Returns the active pin in the multiplexer
int getActivePin(void);

#endif /* MULTIPLEXER_H_ */
