#include "StateMachineFunctions.h"
#include "StateMachineData.h"
#include "StateMachine.h"

#include "FwSmConfig.h"
#include "SysTick.h"
#include "Board.h"
#include "GPIO.h"
#include "Buzzer.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Display.h"



typedef enum { MENU_ADD_ID, MENU_DELETE_ID, MENU_CHANGE_PASS, MENU_NO_OPTION }OPTION;

/** Structure to store the data of a user*/
typedef struct {
	ID number;
	PASSWORD password;
}User;

typedef struct{
	User users[MAX_USERS];	/// Array of users.
	int numberOfUsers;		/// Number of user registered in the system.
	int userIndex;			/// Index to point the user who is logging-in.
	int loginAttempts;		/// Keeps track of number of attempts.
	int countDownTimer;		/// Value in seconds for login max. attempts countdown
	double elapsedTime;
	_Bool clearDisplay;		/// Flag to indicate print() to clear display
	OPTION menuOption;
	unsigned long timeoutCount;
	User tempUser; 			/// Temporal user used register a new user
}StateMachineDataPrivate;


static StateMachineData * data;			///
static FwSmDesc_t sm;					///
static StateMachineDataPrivate _data;	///
static char * dummyPtr;  				/// For strtol()
static void _clearBuffer();				///
static int _searchUser(uint64_t n);		///
static void printPass();				///
static void printSudo();				///


static void SmExecuteFun(void)
{
	FwSmMakeTrans(sm,Execute);
}

void Init(FwSmDesc_t smDesc)
{
	DEBUG_PRINT("CONTROL DE ACCESO.\n");
	data = (StateMachineData*)FwSmGetData(smDesc);
	sm = smDesc;

	/// Load Admin user
	_data.users[0].number = 4517660105004211;// ITBA DIEGO 4517660114820276;
	_data.users[0].password = 12345;
	_data.numberOfUsers++;

	/// Execute the state machine periodically
	sysTickAddCallback(&SmExecuteFun,STATE_MACHINE_EXECUTION_PERIOD);
}

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

void displayBrightness(FwSmDesc_t smDesc)
{
	DEBUG_PRINT2("Brightness: %d\n",getBrightnessLevel());

	displayNum(getBrightnessLevel());
	bip(HIGH_PITCH);
}

void increaseBrightness(FwSmDesc_t smDesc)
{
	DEBUG_PRINT2("Brightness: %d\n", getBrightnessLevel());

	_data.timeoutCount = 0;
	brightnessUp();
	displayNum(getBrightnessLevel());
	bip(HIGH_PITCH);
}

void decreaseBrightness(FwSmDesc_t smDesc)
{
	DEBUG_PRINT2("Brightness: %d\n", getBrightnessLevel());

	_data.timeoutCount = 0;
	brightnessDown();
	displayNum(getBrightnessLevel());
	bip(HIGH_PITCH);
}


FwSmBool_t userRegistered(FwSmDesc_t smDesc)
{
	// Search user using data of magnetic card or keyboard input
	if (data->cardPassed == true)
		_data.userIndex = _searchUser(data->cardNumber);
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

void setCountdown(FwSmDesc_t smDesc)
{
	DEBUG_PRINT2("Set countdown timer to %d seconds\n", _data.countDownTimer);

	_data.countDownTimer = _data.countDownTimer * 2; /// Double the previous time
	if(_data.countDownTimer>MAX_COUNTDOWN_COUNTER)
		_data.countDownTimer=MAX_COUNTDOWN_COUNTER;
	displayNum(_data.countDownTimer);
	bip(LOW_PITCH);
}

void updateCountdown(FwSmDesc_t smDesc)
{
	double elapsedTime = FwSmGetStateExecCnt(smDesc)*STATE_MACHINE_EXECUTION_PERIOD;
	if (elapsedTime != _data.elapsedTime)
	{
		_data.elapsedTime = elapsedTime;
		displayNum(_data.countDownTimer-(int)elapsedTime);
		bip(LOW_PITCH);
		DEBUG_PRINT2("%d\n", _data.countDownTimer-(int)elapsedTime);
		if (elapsedTime == _data.countDownTimer)
			FwSmMakeTrans(smDesc, TIMEOUT);
	}
}

void print(FwSmDesc_t smDesc)
{
	DEBUG_PRINT2('%s', data->buffer);

	_data.timeoutCount = 0;
	bip(HIGH_PITCH);

	/// Check if it is necessary to clear display
	if (_data.clearDisplay == true)
	{
		_data.clearDisplay = false;
		dispClear();
	}
	/// Check state
	int state = FwSmGetCurState(smDesc);
	if (state == StateMachine_PASSWORD_INPUT ||
		state == StateMachine_PASSWORD_INPUT_1 ||
		state == StateMachine_PASSWORD_INPUT_2)
		dispPutchar('-');
	else
		dispPutchar(data->buffer[data->bufferLen - 1]);
}

void printMenu(FwSmDesc_t smDesc)
{
	DEBUG_PRINT("       MENU:\n\Options:\n  1-Add ID \n  2-Change password \n  3-Delete ID \n  *-Back");

	dispMessage(SUDO);
	_clearBuffer();
}

void clearBuffer(FwSmDesc_t smDesc)
{
	_clearBuffer();
}

void clearScreen(FwSmDesc_t smDesc)
{
	DEBUG_PRINT("Clear screen\n");
	dispClear();
}

void printPassAndClear(FwSmDesc_t smDesc)
{
	dispMessage(PASS);
	_data.clearDisplay = 1;
	_clearBuffer();
}

FwSmBool_t passwordOK(FwSmDesc_t smDesc)
{
	/// Convert buffer to number and compare with user password.
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

void countAttempt(FwSmDesc_t smDesc)
{
	DEBUG_PRINT3("%d / %d attempts\n", _data.loginAttempts, MAX_LOGIN_ATTEMPTS);

	_data.loginAttempts++;
	dispMessage(ERR);
	sysTickAddDelayCall(&printPass,0.7);
	_data.clearDisplay=1;
	longBip(LOW_PITCH);
	_clearBuffer();
}

FwSmBool_t maxAttempts(FwSmDesc_t smDesc)
{
	if (_data.loginAttempts + 1 == MAX_LOGIN_ATTEMPTS)
	{
		DEBUG_PRINT("Max attempts reached. Countdown!\n");
		_data.loginAttempts = 0;
		return true;
	}
	else
		return false;
}

void openDoor(FwSmDesc_t smDesc)
{
	DEBUG_PRINT("OPEN DOOR.\n");

	dispOpenDoor();
	longBip(HIGH_PITCH);
	dispMessage(OPEN);
}

void closeDoor(FwSmDesc_t smDesc)
{
	DEBUG_PRINT("CLOSE DOOR.\n");

	bip(LOW_PITCH);
	dispCloseDoor();
	dispClear();
}

FwSmBool_t timeIsUp(FwSmDesc_t smDesc)
{
    DEBUG_PRINT2("%d\n", OPEN_DOOR_TIME - (int)elapsedTime);

	double elapsedTime = FwSmGetStateExecCnt(smDesc)*STATE_MACHINE_EXECUTION_PERIOD;
	if (elapsedTime == OPEN_DOOR_TIME)
		return true;
	else
		return false;
}

FwSmBool_t isAdmin(FwSmDesc_t smDesc)
{
	return (_data.userIndex == 0);
}

FwSmBool_t checkLen(FwSmDesc_t smDesc)
{
	if(data->cardPassed==false && data->bufferLen<8)
		return false;
	else
		return true;
}

FwSmBool_t checkPassLen(FwSmDesc_t smDesc)
{
	return ((MIN_PASS_LEN<=data->bufferLen) && (data->bufferLen<=MAX_PASS_LEN));
}

void checkTimeout(FwSmDesc_t smDesc)
{
	DEBUG_PRINT3("%f - %d \n", elapsedTime, TIMEOUT_TIME);
	
	_data.timeoutCount++;
	double elapsedTime = _data.timeoutCount*STATE_MACHINE_EXECUTION_PERIOD;

	if (elapsedTime >= TIMEOUT_TIME)
		FwSmMakeTrans(smDesc, TIMEOUT);
	
	/// If running state has stopped so go back to idle.
	if(FwSmGetCurStateEmb(smDesc)==0)
		FwSmMakeTrans(smDesc, RESET);
}

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
		data->cardPassed = false;
	}
}

void errorSound(FwSmDesc_t smDesc)
{
	DEBUG_PRINT("EEEEEEE\n");

	longBip(LOW_PITCH);
	dispErrorLed();
}

void soundOK(FwSmDesc_t smDesc)
{
	DEBUG_PRINT("User accepted. \n");

	dispMessage(PASS);
	_data.clearDisplay = 1;
	bip(HIGH_PITCH);
	_clearBuffer();
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

void changePass(FwSmDesc_t smDesc)
{
	DEBUG_PRINT("Update password.\n");
	_data.users[_data.userIndex].password = _data.tempUser.password;
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
		// esta queriendo cambiar la contraseña por haberse llamado
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

static void printPass()
{
	dispMessage(PASS);
}

static void printSudo()
{
	dispMessage(SUDO);
}
