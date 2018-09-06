/**
 * @file CardReader.h
 * @date 5 Sep 2018
 * @brief Header file for usage of the Magnetic Card Reader Driver
 */

#ifndef CARDREADER_H_
#define CARDREADER_H_

/////////////////////////////////////////////////////////////////////////////////
//                             Included header files                           //
/////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>

/////////////////////////////////////////////////////////////////////////////////
//                       Constants and macro definitions                       //
/////////////////////////////////////////////////////////////////////////////////

#define DATA_CARD_NUMBER_LENGTH 19

typedef enum crDataState_ENUM {
	_crNoData,
	_crDataOk,
	_crError
}CR_DATA;


/////////////////////////////////////////////////////////////////////////////////
//                         Global function prototypes                          //
/////////////////////////////////////////////////////////////////////////////////
// Magnetic card reader initialization
void initCardReader(uint8_t enable_pin, uint8_t clock_pin, uint8_t data_pin);

// Interrupt subroutine called when a button is pressed
void magneticCardISR(void);

// Interrupt subroutine called to reset the driver periodically
void magneticCardPISR(void);

// Services
_Bool cardInserted(void);
CR_DATA isDataReady(void);
uint8_t getCardNumber(uint8_t * array);

#endif /* CARDREADER_H_ */
