#pragma once

#include <stdint.h>
#include <stdbool.h>
/** Maximum number of users. */
#define MAX_USERS 20 

/** Size of the buffer for keyboar input. */
#define BUFFER_SIZE 10

/** Number of login attempts before triggering countdown. */
#define MAX_LOGIN_ATTEMPTS 3

/** Start value in seconds of login countown timer. It doubles each time is triggered. */
#define COUNTDOWN_MAX_ATTEMPTS_START_VALUE 10

/** Time in seconds after the last keyboar input to go from RUNNING to IDLE. */
#define TIMEOUT_TIME 60

/** Time in seconds after the last keyboar input to go to from SET_BRIGHTNESS to IDLE. */
#define BRIGHTNESS_TIMEOUT_TIME 3

/** Time in seconds the door remains open. During this time the input is not processed. */
#define OPEN_DOOR_TIME 5

/** Frequency and period in which the FSM is executed*/
#define STATE_MACHINE_EXECUTION_FREQUENCY 1
#define STATE_MACHINE_EXECUTION_PERIOD (1/(double)STATE_MACHINE_EXECUTION_FREQUENCY)

/** IDs are stored as 64 bit integers (up to 19 digits). */
typedef uint64_t ID;

/** Paswords are stored as 32 bit integers (up to 9 digits). */
typedef uint32_t PASSWORD;

/** Structure to store the data of a user*/
typedef struct {
	/***********************************
	Es mejor guardarlo como un arreglo?
	char number[20];
	char password[6];
	***********************************/
	ID number;
	PASSWORD password;
}User;

typedef struct {
	char buffer[BUFFER_SIZE];/// Buffer to store keyboard input.
	int bufferLen; 
	ID cardNumber;			/// Store card number
	bool cardPassed;
}StateMachineData;