/**
 * @file StateMachine.c
 *
 * @author FW Profile code generator version 5.01
 * @date Created on: Sep 6 2018 12:54:51
 */

/** StateMachine function definitions */
#include "StateMachine.h"

/** FW Profile function definitions */
#include "FwSmSCreate.h"
#include "FwSmConfig.h"

#include <stdlib.h>

/** Guard on the transition from CHOICE1 to Final State. */
FwSmBool_t Guard2(FwSmDesc_t smDesc)
{
	return 1;
}

/** Guard on the transition from CHOICE2 to PASSWORD_INPUT. */
FwSmBool_t Guard3(FwSmDesc_t smDesc)
{
	return 1;
}

/**
 * Guard on the transition from CHOICE4 to PASSWORD_INPUT_1.
 * Else
 * @param smDesc the state machine descriptor
 * @return 1 if the guard is fulfilled, otherwise 0.
 */
static FwSmBool_t code45099(FwSmDesc_t smDesc)
{
	return 1;
}

/* ----------------------------------------------------------------------------------------------------------------- */
FwSmDesc_t StateMachineCreate(void* smData)
{
	const FwSmCounterU2_t N_OUT_OF_DOOR_OPENED = 1;	/* The number of transitions out of state DOOR_OPENED */
	const FwSmCounterU2_t N_OUT_OF_ID_INPUT = 3;	/* The number of transitions out of state ID_INPUT */
	const FwSmCounterU2_t CHOICE1 = 1;		/* The identifier of choice pseudo-state CHOICE1 in State Machine StateMachine */
	const FwSmCounterU2_t N_OUT_OF_CHOICE1 = 2;	/* The number of transitions out of the choice-pseudo state CHOICE1 */
	const FwSmCounterU2_t N_OUT_OF_PASSWORD_INPUT = 4;	/* The number of transitions out of state PASSWORD_INPUT */
	const FwSmCounterU2_t CHOICE2 = 2;		/* The identifier of choice pseudo-state CHOICE2 in State Machine StateMachine */
	const FwSmCounterU2_t N_OUT_OF_CHOICE2 = 3;	/* The number of transitions out of the choice-pseudo state CHOICE2 */
	const FwSmCounterU2_t N_OUT_OF_COUNTDOWN = 1;	/* The number of transitions out of state COUNTDOWN */
	const FwSmCounterU2_t N_OUT_OF_PASSWORD_INPUT_1 = 3;	/* The number of transitions out of state PASSWORD_INPUT_1 */
	const FwSmCounterU2_t N_OUT_OF_MENU_ID_INPUT = 4;	/* The number of transitions out of state MENU_ID_INPUT */
	const FwSmCounterU2_t CHOICE4 = 3;		/* The identifier of choice pseudo-state CHOICE4 in State Machine StateMachine */
	const FwSmCounterU2_t N_OUT_OF_CHOICE4 = 3;	/* The number of transitions out of the choice-pseudo state CHOICE4 */
	const FwSmCounterU2_t CHOICE5 = 4;		/* The identifier of choice pseudo-state CHOICE5 in State Machine StateMachine */
	const FwSmCounterU2_t N_OUT_OF_CHOICE5 = 3;	/* The number of transitions out of the choice-pseudo state CHOICE5 */
	const FwSmCounterU2_t N_OUT_OF_PASSWORD_INPUT_2 = 3;	/* The number of transitions out of state PASSWORD_INPUT_2 */
	const FwSmCounterU2_t N_OUT_OF_MENU = 2;	/* The number of transitions out of state MENU */

	/** Create state machine EsmDesc1, which is embedded in RUNNING */
	FW_SM_INST(EsmDesc1,
		8,	/* NSTATES - The number of states */
		4,	/* NCPS - The number of choice pseudo-states */
		33,	/* NTRANS - The number of transitions */
		20,	/* NACTIONS - The number of state and transition actions */
		13	/* NGUARDS - The number of transition guards */
	);
	FwSmInit(&EsmDesc1);

	/** Configure the state machine EsmDesc1, which is embedded in RUNNING */
	FwSmSetData(&EsmDesc1, smData);
	FwSmAddState(&EsmDesc1, StateMachine_DOOR_OPENED, N_OUT_OF_DOOR_OPENED, &openDoor, &closeDoor, NULL, NULL);
	FwSmAddState(&EsmDesc1, StateMachine_ID_INPUT, N_OUT_OF_ID_INPUT, &checkForCard, NULL, NULL, NULL);
	FwSmAddChoicePseudoState(&EsmDesc1, CHOICE1, N_OUT_OF_CHOICE1);
	FwSmAddState(&EsmDesc1, StateMachine_PASSWORD_INPUT, N_OUT_OF_PASSWORD_INPUT, NULL, NULL, NULL, NULL);
	FwSmAddChoicePseudoState(&EsmDesc1, CHOICE2, N_OUT_OF_CHOICE2);
	FwSmAddState(&EsmDesc1, StateMachine_COUNTDOWN, N_OUT_OF_COUNTDOWN, &setCountdown, NULL, &updateCountdown, NULL);
	FwSmAddState(&EsmDesc1, StateMachine_PASSWORD_INPUT_1, N_OUT_OF_PASSWORD_INPUT_1, NULL, NULL, NULL, NULL);
	FwSmAddState(&EsmDesc1, StateMachine_MENU_ID_INPUT, N_OUT_OF_MENU_ID_INPUT, NULL, NULL, NULL, NULL);
	FwSmAddChoicePseudoState(&EsmDesc1, CHOICE4, N_OUT_OF_CHOICE4);
	FwSmAddChoicePseudoState(&EsmDesc1, CHOICE5, N_OUT_OF_CHOICE5);
	FwSmAddState(&EsmDesc1, StateMachine_PASSWORD_INPUT_2, N_OUT_OF_PASSWORD_INPUT_2, NULL, NULL, NULL, NULL);
	FwSmAddState(&EsmDesc1, StateMachine_MENU, N_OUT_OF_MENU, &printMenu, NULL, NULL, NULL);
	FwSmAddTransIpsToSta(&EsmDesc1, StateMachine_ID_INPUT, NULL);
	FwSmAddTransStaToFps(&EsmDesc1, Execute, StateMachine_DOOR_OPENED, NULL, &timeIsUp);
	FwSmAddTransStaToSta(&EsmDesc1, NUMBER, StateMachine_ID_INPUT, StateMachine_ID_INPUT, &print, NULL);
	FwSmAddTransStaToSta(&EsmDesc1, KEY_1, StateMachine_ID_INPUT, StateMachine_ID_INPUT, &erase, NULL);
	FwSmAddTransStaToCps(&EsmDesc1, KEY_2, StateMachine_ID_INPUT, CHOICE1, NULL, NULL);
	FwSmAddTransCpsToSta(&EsmDesc1, CHOICE1, StateMachine_PASSWORD_INPUT, &soundOK, &userRegistered);
	FwSmAddTransCpsToFps(&EsmDesc1, CHOICE1, &errorSound, &Guard2);
	FwSmAddTransStaToSta(&EsmDesc1, NUMBER, StateMachine_PASSWORD_INPUT, StateMachine_PASSWORD_INPUT, &print, NULL);
	FwSmAddTransStaToSta(&EsmDesc1, KEY_1, StateMachine_PASSWORD_INPUT, StateMachine_PASSWORD_INPUT, &erase, NULL);
	FwSmAddTransStaToCps(&EsmDesc1, KEY_2, StateMachine_PASSWORD_INPUT, CHOICE2, NULL, NULL);
	FwSmAddTransStaToSta(&EsmDesc1, LOGIN, StateMachine_PASSWORD_INPUT, StateMachine_MENU, NULL, &isAdmin);
	FwSmAddTransCpsToSta(&EsmDesc1, CHOICE2, StateMachine_DOOR_OPENED, NULL, &passwordOK);
	FwSmAddTransCpsToSta(&EsmDesc1, CHOICE2, StateMachine_COUNTDOWN, NULL, &maxAttempts);
	FwSmAddTransCpsToSta(&EsmDesc1, CHOICE2, StateMachine_PASSWORD_INPUT, &countAttempt, &Guard3);
	FwSmAddTransStaToSta(&EsmDesc1, TIMEOUT, StateMachine_COUNTDOWN, StateMachine_PASSWORD_INPUT, &clrBufferScreen, NULL);
	FwSmAddTransStaToSta(&EsmDesc1, NUMBER, StateMachine_PASSWORD_INPUT_1, StateMachine_PASSWORD_INPUT_1, &print, NULL);
	FwSmAddTransStaToSta(&EsmDesc1, KEY_2, StateMachine_PASSWORD_INPUT_1, StateMachine_PASSWORD_INPUT_2, &storePass, NULL);
	FwSmAddTransStaToSta(&EsmDesc1, KEY_1, StateMachine_PASSWORD_INPUT_1, StateMachine_PASSWORD_INPUT_1, &erase, NULL);
	FwSmAddTransStaToSta(&EsmDesc1, NUMBER, StateMachine_MENU_ID_INPUT, StateMachine_MENU_ID_INPUT, &print, NULL);
	FwSmAddTransStaToSta(&EsmDesc1, KEY_1, StateMachine_MENU_ID_INPUT, StateMachine_MENU_ID_INPUT, &erase, NULL);
	FwSmAddTransStaToCps(&EsmDesc1, KEY_2, StateMachine_MENU_ID_INPUT, CHOICE4, NULL, NULL);
	FwSmAddTransStaToCps(&EsmDesc1, CARD, StateMachine_MENU_ID_INPUT, CHOICE4, NULL, NULL);
	FwSmAddTransCpsToSta(&EsmDesc1, CHOICE4, StateMachine_MENU, &displayIDError, &errorWithID);
	FwSmAddTransCpsToSta(&EsmDesc1, CHOICE4, StateMachine_MENU, &deleteID, &isDeletingID);
	FwSmAddTransCpsToSta(&EsmDesc1, CHOICE4, StateMachine_PASSWORD_INPUT_1, &storeID, &code45099);
	FwSmAddTransCpsToSta(&EsmDesc1, CHOICE5, StateMachine_PASSWORD_INPUT_1, &displayPasswordMatchError, &passNotMatch);
	FwSmAddTransCpsToSta(&EsmDesc1, CHOICE5, StateMachine_MENU, &addID, &isAddingID);
	FwSmAddTransCpsToSta(&EsmDesc1, CHOICE5, StateMachine_MENU, &changePass, &isChangingPass);
	FwSmAddTransStaToSta(&EsmDesc1, NUMBER, StateMachine_PASSWORD_INPUT_2, StateMachine_PASSWORD_INPUT_2, &print, NULL);
	FwSmAddTransStaToSta(&EsmDesc1, KEY_1, StateMachine_PASSWORD_INPUT_2, StateMachine_PASSWORD_INPUT_2, &erase, NULL);
	FwSmAddTransStaToCps(&EsmDesc1, KEY_2, StateMachine_PASSWORD_INPUT_2, CHOICE5, NULL, NULL);
	FwSmAddTransStaToFps(&EsmDesc1, KEY_2, StateMachine_MENU, NULL, NULL);
	FwSmAddTransStaToSta(&EsmDesc1, NUMBER, StateMachine_MENU, StateMachine_MENU_ID_INPUT, &storeOption, NULL);

	const FwSmCounterU2_t N_OUT_OF_RUNNING = 2;	/* The number of transitions out of state RUNNING */
	const FwSmCounterU2_t N_OUT_OF_IDLE = 3;	/* The number of transitions out of state IDLE */
	const FwSmCounterU2_t N_OUT_OF_SET_BRIGHTNESS = 3;	/* The number of transitions out of state SET_BRIGHTNESS */

	/** Create state machine smDesc */
	FW_SM_INST(smDesc,
		3,	/* NSTATES - The number of states */
		0,	/* NCPS - The number of choice pseudo-states */
		9,	/* NTRANS - The number of transitions */
		9,	/* NACTIONS - The number of state and transition actions */
		0	/* NGUARDS - The number of transition guards */
	);
	FwSmInit(&smDesc);

	/** Configure the state machine smDesc */
	FwSmSetData(&smDesc, smData);
	FwSmAddState(&smDesc, StateMachine_RUNNING, N_OUT_OF_RUNNING, NULL, NULL, &checkTimeout, &EsmDesc1);
	FwSmAddState(&smDesc, StateMachine_IDLE, N_OUT_OF_IDLE, &reset, NULL, NULL, NULL);
	FwSmAddState(&smDesc, StateMachine_SET_BRIGHTNESS, N_OUT_OF_SET_BRIGHTNESS, NULL, NULL, &checkTimeout2, NULL);
	FwSmAddTransIpsToSta(&smDesc, StateMachine_IDLE, &Init);
	FwSmAddTransStaToSta(&smDesc, TIMEOUT, StateMachine_RUNNING, StateMachine_IDLE, &bipSound, NULL);
	FwSmAddTransStaToSta(&smDesc, RESET, StateMachine_RUNNING, StateMachine_IDLE, NULL, NULL);
	FwSmAddTransStaToSta(&smDesc, NUMBER, StateMachine_IDLE, StateMachine_RUNNING, &print, NULL);
	FwSmAddTransStaToSta(&smDesc, SET_BRIGHTNESS, StateMachine_IDLE, StateMachine_SET_BRIGHTNESS, &displayBrightness, NULL);
	FwSmAddTransStaToSta(&smDesc, CARD, StateMachine_IDLE, StateMachine_RUNNING, NULL, NULL);
	FwSmAddTransStaToSta(&smDesc, TIMEOUT, StateMachine_SET_BRIGHTNESS, StateMachine_IDLE, &bipSound, NULL);
	FwSmAddTransStaToSta(&smDesc, KEY_2, StateMachine_SET_BRIGHTNESS, StateMachine_SET_BRIGHTNESS, &increaseBrightness, NULL);
	FwSmAddTransStaToSta(&smDesc, KEY_1, StateMachine_SET_BRIGHTNESS, StateMachine_SET_BRIGHTNESS, &decreaseBrightness, NULL);

	return &smDesc;
}
