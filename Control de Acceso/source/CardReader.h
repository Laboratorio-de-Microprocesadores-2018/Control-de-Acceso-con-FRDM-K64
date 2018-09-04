/* ____________________________________________________________________________
  /\               ITBA - Laboratorio de Microprocesadores - 2018              \
  \_|                                 Grupo 3                                  |  
    |	CardReader.h                                                           |  
    |                                                                          |  
    |  ________________________________________________________________________|_ 
    \_/_________________________________________________________________________/
*/
#ifndef CARDREADER_H_
#define CARDREADER_H_

#include <stdint.h>

// Magnetic card reader initialization
void initCardReader(void);

// Interrupt subroutine called when a button is pressed
void magneticCardISR(void);

// Interrupt subroutine called to reset the driver periodically
void magneticCardPISR(void);

// Services
_Bool cardInserted(void);
_Bool isDataReady(void);
_Bool getData(uint8_t* data);

#endif /* CARDREADER_H_ */
