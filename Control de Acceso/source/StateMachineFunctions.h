#ifndef STATEMACHINEFUNCTIONS_H_
#define STATEMACHINEFUNCTIONS_H_


#include "FwSmConstants.h"

//#define DEBUG_PRINT_ON

#ifdef DEBUG_PRINT_ON
	#define DEBUG_PRINT(a) printf(a)
	#define DEBUG_PRINT2(a,b) printf(a,b)
	#define DEBUG_PRINT3(a,b,c) printf(a,b,c)
#else
	#define DEBUG_PRINT(a)
	#define DEBUG_PRINT2(a,b)
	#define DEBUG_PRINT3(a,b,c)
#endif

/**
*
* @param smDesc the state machine descriptor
*/
void clearBuffer(FwSmDesc_t smDesc);

/**
* Action on the transition from PASSWORD_INPUT_1 to PASSWORD_INPUT_2.
* @param smDesc the state machine descriptor
*/
void clearScreen(FwSmDesc_t smDesc);

/**
*
* @param smDesc the state machine descriptor
*/
void clrBufferScreen(FwSmDesc_t smDesc);


/**
* Guard on the transition from CHOICE4 to PASSWORD_INPUT2.
* @param smDesc the state machine descriptor
* @return 1 if the guard is fulfilled, otherwise 0.
*/
FwSmBool_t userNotRegistered(FwSmDesc_t smDesc);

/**
* Guard on the transition from CHOICE1 to PASSWORD_INPUT.
* @param smDesc the state machine descriptor
* @return 1 if the guard is fulfilled, otherwise 0.
*/
FwSmBool_t userRegistered(FwSmDesc_t smDesc);

/**
* Guard on the transition from CHOICE2 to Final State.
* @param smDesc the state machine descriptor
* @return 1 if the guard is fulfilled, otherwise 0.
*/
FwSmBool_t passwordOK(FwSmDesc_t smDesc);

/**
* Guard on the transition from CHOICE2 to COUNTDOWN.
* @param smDesc the state machine descriptor
* @return 1 if the guard is fulfilled, otherwise 0.
*/
FwSmBool_t maxAttempts(FwSmDesc_t smDesc);

/**
* Action on the transition from CHOICE2 to PASSWORD_INPUT.
* @param smDesc the state machine descriptor
*/
void countAttempt(FwSmDesc_t smDesc);



/**
* Entry Action for the state OPEN_DOOR.
* @param smDesc the state machine descriptor
*/
void openDoor(FwSmDesc_t smDesc);

/**
* Exit Action for the state OPEN_DOOR.
* @param smDesc the state machine descriptor
*/
void closeDoor(FwSmDesc_t smDesc);

/**
* Do Action for the state OPEN_DOOR.
* @param smDesc the state machine descriptor
*/
FwSmBool_t timeIsUp(FwSmDesc_t smDesc);

/**
* Guard on the transition from INPUT_OPTION to ADD_ID.
* @param smDesc the state machine descriptor
* @return 1 if the guard is fulfilled, otherwise 0.
*/
FwSmBool_t isAdmin(FwSmDesc_t smDesc);

/**
* Entry Action for the state IDLE.
* @param smDesc the state machine descriptor
*/
void reset(FwSmDesc_t smDesc);

void stop(FwSmDesc_t smDesc);
/**
* Action on the transition from Initial State to .
* @param smDesc the state machine descriptor
*/
void Init(FwSmDesc_t smDesc);

/**
* Do Action for the state .
* @param smDesc the state machine descriptor
*/
void checkTimeout(FwSmDesc_t smDesc);
void checkTimeout2(FwSmDesc_t smDesc);
void storeID(FwSmDesc_t smDesc);
void storePass(FwSmDesc_t smDesc);


/**
* Entry Action for the state KEYBOARD_ID.
* @param smDesc the state machine descriptor
*/
void checkForCard(FwSmDesc_t smDesc);

/**
* Entry action for the state COUNTDOWN.
* @param smDesc the state machine descriptor
*/
void setCountdown(FwSmDesc_t smDesc);

/**
* Do Action for the state COUNTDOWN.
* @param smDesc the state machine descriptor
*/
void updateCountdown(FwSmDesc_t smDesc);

/**
* 
* @param smDesc the state machine descriptor
*/

void print(FwSmDesc_t smDesc);
void printMenu(FwSmDesc_t smDesc);
/**
*
* @param smDesc the state machine descriptor
*/
void errorSound(FwSmDesc_t smDesc);

/**
*
* @param smDesc the state machine descriptor
*/
void bipSound(FwSmDesc_t smDesc);

/**
*
* @param smDesc the state machine descriptor
*/


FwSmBool_t IDlengthOK(FwSmDesc_t smDesc);

/**
*
* @param smDesc the state machine descriptor
* @return 1 if the guard is fulfilled, otherwise 0.
*/
FwSmBool_t errorWithID(FwSmDesc_t smDesc);

/**
*
* @param smDesc the state machine descriptor
* @return 1 if the guard is fulfilled, otherwise 0.
*/
FwSmBool_t isAddingID(FwSmDesc_t smDesc);

/**
*
* @param smDesc the state machine descriptor
* @return 1 if the guard is fulfilled, otherwise 0.
*/
FwSmBool_t isDeletingID(FwSmDesc_t smDesc);

/**
*
* @param smDesc the state machine descriptor
* @return 1 if the guard is fulfilled, otherwise 0.
*/
FwSmBool_t passNotMatch(FwSmDesc_t smDesc);

/**
* 
* @param smDesc the state machine descriptor
* @return 1 if the guard is fulfilled, otherwise 0.
*/
FwSmBool_t isChangingPass(FwSmDesc_t smDesc);

/**
* 
* @param smDesc the state machine descriptor
* @return 1 if the guard is fulfilled, otherwise 0.
*/
FwSmBool_t keyboardInput(FwSmDesc_t smDesc);

/**
*
* @param smDesc the state machine descriptor
*/
void erase(FwSmDesc_t smDesc);

/**
*
* @param smDesc the state machine descriptor
*/
void storeOption(FwSmDesc_t smDesc);

/**
*
* @param smDesc the state machine descriptor
*/
void displayIDError(FwSmDesc_t smDesc);

/**
*
* @param smDesc the state machine descriptor
*/
void addID(FwSmDesc_t smDesc);

/**
*
* @param smDesc the state machine descriptor
*/
void deleteID(FwSmDesc_t smDesc);

/**
*
* @param smDesc the state machine descriptor
*/
void displayPasswordMatchError(FwSmDesc_t smDesc);

/**
*
* @param smDesc the state machine descriptor
*/
void changePass(FwSmDesc_t smDesc);

/**
*
* @param smDesc the state machine descriptor
*/
void increaseBrightness(FwSmDesc_t smDesc);

/**
*
* @param smDesc the state machine descriptor
*/
void decreaseBrightness(FwSmDesc_t smDesc);

/**
*
* @param smDesc the state machine descriptor
*/
void displayBrightness(FwSmDesc_t smDesc);

#endif
