#include "StateMachineFunctions.h"
#include "StateMachineData.h"
#include "StateMachine.h" // CAMBIAR DESPES POR StateMachineEvents?
//#include "FwSmCore.h"
#include "FwSmConfig.h"
#include "SysTick.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Display.h"
#include "Board.h"
#include "GPIO.h"
#include "Buzzer.h"


#define DOOR_LED PIN_LED_GREEN

/* ----------------------------------------------------------------------------------------------------------------- */
typedef enum { MENU_ADD_ID, MENU_DELETE_ID, MENU_CHANGE_PASS, MENU_NO_OPTION }OPTION;
typedef struct{
	User users[MAX_USERS];	 /// Array of users.
	int numberOfUsers;		 /// Number of user registered in the system.

	int userIndex;			/// Index to point the user who is logging-in.
	int loginAttempts;		/// Keeps track of number of attempts.
	int countDownTimer;		/// Value in seconds for login max. attempts countdown
	double elapsedTime;
	int displayBrightness;
	//FLAGS
	bool clearDisplay;
	OPTION menuOption;
	unsigned long timeoutCount;
	User tempUser; /// Temporal user used register a new user
}StateMachineDataPrivate;

/** Pointer to public state machine data. */
static StateMachineData * data;
static FwSmDesc_t sm;
static StateMachineDataPrivate _data;
static char * dummyPtr;  // For strtol()
static void _clearBuffer();
static int _searchUser(uint64_t n);



void SmExecuteFun(void)
{
	FwSmMakeTrans(sm,Execute);
}
void toggleRed();

/** Action on the transition from Initial State to IDLE. */
void Init(FwSmDesc_t smDesc)
{
	DEBUG_PRINT("CONTROL DE ACCESO.\n");
	data = (StateMachineData*)FwSmGetData(smDesc);
	sm = smDesc;
	// Admin user
	_data.users[0].number = 4517660105004211;// ITBA DIEGO 4517660114820276;
	_data.users[0].password = 12345;
	_data.numberOfUsers++;
	_data.displayBrightness = 100;

	sysTickAddCallback(&SmExecuteFun,1);

}




void displayBrightness(FwSmDesc_t smDesc)
{
	DEBUG_PRINT2("Brightness: %d\n",_data.displayBrightness);
	displayNum(getBrightnessLevel());
	bip(HIGH_PITCH);
}


void increaseBrightness(FwSmDesc_t smDesc)
{
	_data.timeoutCount = 0;
	DEBUG_PRINT2("Brightness: %d\n", _data.displayBrightness);
	brightnessUp();
	displayNum(getBrightnessLevel());
	bip(HIGH_PITCH);
}


void decreaseBrightness(FwSmDesc_t smDesc)
{
	_data.timeoutCount = 0;
	DEBUG_PRINT2("Brightness: %d\n", _data.displayBrightness);
	brightnessDown();
	displayNum(getBrightnessLevel());
	bip(HIGH_PITCH);
}



/** Guard on the transition from CHOICE1 to PASSWORD_INPUT. */
FwSmBool_t userRegistered(FwSmDesc_t smDesc)
{
	// Search user using data of magnetic card or keyboard input
	if (data->cardPassed == true)
	{
		_data.userIndex = _searchUser(data->cardNumber);
	}
	else
		_data.userIndex = _searchUser(strtol(data->buffer,&dummyPtr,10));


	if (_data.userIndex == -1)
	{
		DEBUG_PRINT("User not registered.\n");
		return false;
	}
	else
	{
		DEBUG_PRINT("User registered.\n");
		return true;
	}
}

/** Entry Action for the state COUNTDOWN. */
void setCountdown(FwSmDesc_t smDesc)
{
	_data.countDownTimer = _data.countDownTimer * 2;
	DEBUG_PRINT2("Set countdown timer to %d seconds\n", _data.countDownTimer);
}

/** Do Action for the state COUNTDOWN. */
void updateCountdown(FwSmDesc_t smDesc)
{
	double elapsedTime = FwSmGetStateExecCnt(smDesc)*STATE_MACHINE_EXECUTION_PERIOD;
	if (elapsedTime != _data.elapsedTime) // Si EXECUTION PERIOD ES 1 SEGUNDO ESTO NO HACE FALTA
	{
		_data.elapsedTime = elapsedTime;
		// ACTUALIZAR DISPLAY!!!
		displayNum(_data.countDownTimer-(int)elapsedTime);
		bip(LOW_PITCH);
		DEBUG_PRINT2("%d\n", _data.countDownTimer-(int)elapsedTime);
		if (elapsedTime == _data.countDownTimer)
			FwSmMakeTrans(smDesc, TIMEOUT);
	}
}

void print(FwSmDesc_t smDesc)
{
	_data.timeoutCount = 0;
	bip(HIGH_PITCH);
	if (_data.clearDisplay == true)
	{
		_data.clearDisplay = false;
		dispClear();
	}
	int state = FwSmGetCurState(smDesc);
	if (state == StateMachine_PASSWORD_INPUT ||
		state == StateMachine_PASSWORD_INPUT_1 ||
		state == StateMachine_PASSWORD_INPUT_2)

	{
		dispPutchar('-');
		DEBUG_PRINT3("%.*s", data->bufferLen, "----------");
	}
	else
	{
		dispPutchar(data->buffer[data->bufferLen - 1]);
		//DEBUG_PRINT("putchar(%c)\n", data->buffer[data->bufferLen - 1]);
		DEBUG_PRINT2('%s', data->buffer);
	}
}

void printMenu(FwSmDesc_t smDesc)
{
	dispMessage(SUDO);
	_clearBuffer();
	DEBUG_PRINT("       MENU:\n\Options:\n  1-Add ID \n  2-Change password \n  3-Delete ID \n  *-Back");

}
void clearBuffer(FwSmDesc_t smDesc)
{
	_clearBuffer();
}

/** Action on the transition from PASSWORD_INPUT_1 to PASSWORD_INPUT_2. */
void clearScreen(FwSmDesc_t smDesc)
{
	DEBUG_PRINT("Clear screen\n");
	dispClear();
}

void clrBufferScreen(FwSmDesc_t smDesc)
{
	_clearBuffer();
	dispClear();
}

/** Guard on the transition from CHOICE2 to Final State. */
FwSmBool_t passwordOK(FwSmDesc_t smDesc)
{
	uint64_t password = strtol(data->buffer,&dummyPtr,10);
	if (_data.users[_data.userIndex].password == password)
	{
		DEBUG_PRINT("Password OK.\n");
		return true;
	}
	else
	{
		DEBUG_PRINT("Wrong password.\n");
		return false;
	}
}

/** Action on the transition from CHOICE2 to PASSWORD_INPUT. */
void countAttempt(FwSmDesc_t smDesc)
{
	_data.loginAttempts++;
	DEBUG_PRINT3("%d / %d attempts\n", _data.loginAttempts, MAX_LOGIN_ATTEMPTS);
	dispMessage(ERR);
	longBip(LOW_PITCH);
	_data.clearDisplay=1;
	_clearBuffer();
}

/** Guard on the transition from CHOICE2 to COUNTDOWN. */
FwSmBool_t maxAttempts(FwSmDesc_t smDesc)
{
	if (_data.loginAttempts + 1 == MAX_LOGIN_ATTEMPTS)
	{
		DEBUG_PRINT("Max attempts reached. Countdown!\n");
		_data.loginAttempts = 0;
		// HABRIA QUE CONTAR LAS VECES QUE SE EJECUTA EL TIMER COMO PARA EN ALGUN PUNTO VOLVER A IDLE????
		return true;
	}
	else
		return false;
}

/** Entry Action for the state OPEN_DOOR. */
void openDoor(FwSmDesc_t smDesc)
{
	DEBUG_PRINT("OPEN DOOR.\n");
	digitalWrite(DOOR_LED,0);
	dispMessage(OPEN);
}

/** Exit Action for the state OPEN_DOOR. */
void closeDoor(FwSmDesc_t smDesc)
{
	DEBUG_PRINT("CLOSE DOOR.\n");
	digitalWrite(DOOR_LED,1);
	dispClear();
}

/** Do Action for the state OPEN_DOOR. */
FwSmBool_t timeIsUp(FwSmDesc_t smDesc)
{
	double elapsedTime = FwSmGetStateExecCnt(smDesc)*STATE_MACHINE_EXECUTION_PERIOD;
    DEBUG_PRINT2("%d\n", OPEN_DOOR_TIME - (int)elapsedTime);
	if (elapsedTime == OPEN_DOOR_TIME)
		return true;//FwSmMakeTrans(smDesc, TIMEOUT);
	else
		return false;
}

/** Guard on the transition from INPUT_OPTION to ADD_ID. */
FwSmBool_t isAdmin(FwSmDesc_t smDesc)
{
	return (_data.userIndex == 0);
}

/** Entry Action for the state IDLE. */
void reset(FwSmDesc_t smDesc)
{
	DEBUG_PRINT("Reset state machine DATA.\n");
	
	_data.countDownTimer = COUNTDOWN_MAX_ATTEMPTS_START_VALUE / 2;
	_data.loginAttempts = 0;
	_data.userIndex = -1;
	_data.timeoutCount = 0;

	_data.menuOption = MENU_NO_OPTION;

	_clearBuffer();
	dispClear();
	doubleBip(HIGH_PITCH);
}

/** Do Action for the state . */
void checkTimeout(FwSmDesc_t smDesc)
{
	_data.timeoutCount++;
	double elapsedTime = _data.timeoutCount*STATE_MACHINE_EXECUTION_PERIOD;
	//DEBUG_PRINT("%f - %d \n", elapsedTime, TIMEOUT_TIME);
	if (elapsedTime >= TIMEOUT_TIME)
		FwSmMakeTrans(smDesc, TIMEOUT);
	
	if(FwSmGetCurStateEmb(smDesc)==0) // Running state has stopped so go back to idle
		FwSmMakeTrans(smDesc, RESET);
}
/** Do Action for the state SET_BRIGHTNESS. */
void checkTimeout2(FwSmDesc_t smDesc)
{
	_data.timeoutCount++;
	double elapsedTime = _data.timeoutCount*STATE_MACHINE_EXECUTION_PERIOD;
	if (elapsedTime >= BRIGHTNESS_TIMEOUT_TIME)
		FwSmMakeTrans(smDesc, TIMEOUT);
}

void checkForCard(FwSmDesc_t smDesc)
{
	if(data->cardPassed==true && data->bufferLen==0)
	{
		FwSmMakeTrans(smDesc, KEY_2);
		data->cardPassed=false;
	}
}


void errorSound(FwSmDesc_t smDesc)
{
	DEBUG_PRINT("EEEEEE\n");
	longBip(LOW_PITCH);
	//digitalWrite(PIN_LED_RED,0);
	//sysTickAddDelayCall(&toggleRed,1);
}

void soundOK(FwSmDesc_t smDesc)
{
	DEBUG_PRINT("User accepted\n");
	dispMessage(PASS);
	_data.clearDisplay = 1;
	bip(HIGH_PITCH);
	_clearBuffer();
}

void toggleRed()
{
	digitalToggle(PIN_LED_RED);
}

void bipSound(FwSmDesc_t smDesc)
{
	DEBUG_PRINT("BIPPPPP\n");
	bip(HIGH_PITCH);
}

void storeOption(FwSmDesc_t smDesc)
{
	switch (data->buffer[0])
	{
	case '1':
		_data.menuOption = MENU_ADD_ID;
		dispMessage(ADD);
		DEBUG_PRINT("Adding new ID. First input the new ID number or pass magnetic card.\n");
		break;
	case '2':
		_data.menuOption = MENU_CHANGE_PASS;
		dispMessage(EDIT);
		DEBUG_PRINT("Changing pass. First input the ID number or pass magnetic card.\n");
		break;
	case '3':
		_data.menuOption = MENU_DELETE_ID;
		dispMessage(DEL);
		DEBUG_PRINT("Deleting ID. Input the ID you want to delete or pass magnetic card.\n");
		break;
	}
	_data.clearDisplay = true;
	_clearBuffer();
}
void displayIDError(FwSmDesc_t smDesc)
{
	int registered = userRegistered(smDesc);
	if (registered &&_data.menuOption == MENU_ADD_ID)
	{
		DEBUG_PRINT("ERROR: This user is already registered! .\n");
		//dispError(1);
	}
	else if (!registered && _data.menuOption == MENU_CHANGE_PASS)
	{
		DEBUG_PRINT("ERROR: This user is not registered!.\n");
		//dispError(2);
	}
	else if (!registered &&_data.menuOption == MENU_DELETE_ID)
	{
		DEBUG_PRINT("ERROR: This user is not registered!.\n");
		//dispError(3);
	}
	dispMessage(ERR);
	sysTickAddDelayCall(&printMenu,0.7);

	longBip(LOW_PITCH);
	_data.clearDisplay = true;
	_clearBuffer();
}

void addID(FwSmDesc_t smDesc)
{
	DEBUG_PRINT("Adding new user...\n");
	if (_data.numberOfUsers < MAX_USERS)
	{
		_data.users[_data.numberOfUsers] = _data.tempUser;
		_data.numberOfUsers++;
	}
	else
		DEBUG_PRINT("Cant add new user. MAX USERS\n");
}
void deleteID(FwSmDesc_t smDesc)
{
	DEBUG_PRINT("Deleting ID...\n");
	for (int i = _data.userIndex; i + 1 < _data.numberOfUsers; i++)
		_data.users[i] = _data.users[i + 1];
	_data.numberOfUsers--;
}

void displayPasswordMatchError(FwSmDesc_t smDesc)
{
	DEBUG_PRINT("Passwords did not match! Try again.\n");
	longBip(LOW_PITCH);
	_data.tempUser.password = 0;
	_clearBuffer();
	dispMessage(PASS);
	_data.clearDisplay=1;
}

void changePass(FwSmDesc_t smDesc)
{
	DEBUG_PRINT("Update password.\n");
	_data.users[_data.userIndex].password = _data.tempUser.password;
}

void storeID(FwSmDesc_t smDesc)
{
	if (_data.menuOption == MENU_ADD_ID)
	{
		if (data->cardPassed == true)
		{
			_data.tempUser.number = data->cardNumber;
			data->cardPassed = false;
		}
		else
			_data.tempUser.number = strtol(data->buffer,&dummyPtr,10);
	}
	else if (_data.menuOption == MENU_CHANGE_PASS)
	{
		// En _data.userIndex va a estar el indice al usuario que
		// esta queriendo cambiar la contrase�a por haberse llamado
		// antes al guardia "errorWithID"
	}

	bip(HIGH_PITCH);
	dispMessage(PASS);
	_data.clearDisplay=1;
	_clearBuffer();
}
void storePass(FwSmDesc_t smDesc)
{
	_data.tempUser.password = strtol(data->buffer,&dummyPtr,10);

	bip(HIGH_PITCH);
	dispMessage(PASS);
	_data.clearDisplay=1;
	_clearBuffer();
}

FwSmBool_t errorWithID(FwSmDesc_t smDesc)
{
	int registered = userRegistered(smDesc);

	if ((registered && _data.menuOption==MENU_ADD_ID) ||
		(!registered && _data.menuOption==MENU_CHANGE_PASS) ||
		(!registered && _data.menuOption == MENU_DELETE_ID) ||
		(_data.userIndex==0 &&_data.menuOption == MENU_DELETE_ID ))
		return 1;
	else
		return 0;
}

FwSmBool_t isAddingID(FwSmDesc_t smDesc)
{
	return (_data.menuOption == MENU_ADD_ID);
}

FwSmBool_t isChangingPass(FwSmDesc_t smDesc)
{
	return (_data.menuOption == MENU_CHANGE_PASS);
}

FwSmBool_t isDeletingID(FwSmDesc_t smDesc)
{
	return (_data.menuOption == MENU_DELETE_ID);
}


FwSmBool_t passNotMatch(FwSmDesc_t smDesc)
{
	return (_data.tempUser.password != strtol(data->buffer,&dummyPtr,10));
}

void erase(FwSmDesc_t smDesc)
{
	DEBUG_PRINT2('%s', data->buffer);
	if (data->bufferLen > 0)
	{
		bip(HIGH_PITCH);
		data->bufferLen--;	
		data->buffer[data->bufferLen] = 0;
		dispErase();
	}
}

static void _clearBuffer()
{
	memset(data->buffer, 0, BUFFER_SIZE);
	data->bufferLen = 0;
	DEBUG_PRINT("Clear buffer\n");
}

static int _searchUser(uint64_t n)
{
	for (int i = 0; i < _data.numberOfUsers; i++)
		if (_data.users[i].number == n)
			return i;
	return -1;
}
