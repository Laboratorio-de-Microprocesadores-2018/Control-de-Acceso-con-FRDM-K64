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


#define PIN_DATA PORTNUM2PIN(PC,4)
#define PIN_CLOCK PORTNUM2PIN(PD,0)
#define PIN_ENABLE PORTNUM2PIN(PD,2)

typedef enum crDataState_ENUM {
	_crNoData,
	_crDataOk,
	_crError
}CR_DATA;


/////////////////////////////////////////////////////////////////////////////////
//                         Global function prototypes                          //
/////////////////////////////////////////////////////////////////////////////////
// Magnetic card reader initialization
void initCardReader();


// Services
_Bool cardInserted(void);
CR_DATA isDataReady(void);
uint8_t getCardString(uint8_t * array);
uint64_t getCardNumber();

#endif /* CARDREADER_H_ */
