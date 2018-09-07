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

/**
 * @brief Function to check if a card is being read
 *
 * @return Returns true if a card is being read. False otherwise.
 */
_Bool cardInserted(void);

/**
 * @brief Function to check if card information has been read.
 *
 * Function to check if card information has been read. This is the
 * function to call periodically for new data.
 *
 * @return Returns data type CR_DATA which needs to be checked for error in
 * readings or for new card data.
 */
CR_DATA isDataReady(void);


/**
 * @brief Function to copy the card number to a given array.
 *
 * Copies the card number data into the array given. The data is saved as binary numbers.
 * @param array Destination array to copy the card number. Must be at least DATA_CARD_NUMBER_LENGTH cells long.
 * @return Returns the amount of characters copied. If zero, either there was an error
 * in the card data, the data was not ready or the array given was null.
 */
uint8_t getCardString(uint8_t * array);

/**
 * @brief Function to get the card number data in equivalent decimal form.
 *
 * Transforms the binary numbers copied from the card into
 * its decimal equivalent.
 * @return Returns the card number in equivalent decimal value.
 */
uint64_t getCardNumber();

#endif /* CARDREADER_H_ */
