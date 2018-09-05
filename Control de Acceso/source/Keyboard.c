/////////////////////////////////////////////////////////////////////////////////
//                             Included header files                           //
/////////////////////////////////////////////////////////////////////////////////
#include "Keyboard.h"
#include "GPIO.h"
#include "Systick.h"
#include "Multiplexer.h"
#include "CircularBuffer.h"

/////////////////////////////////////////////////////////////////////////////////
//                       Constants and macro definitions                       //
/////////////////////////////////////////////////////////////////////////////////

#define NCOLS 3
#define NROWS 4

#define COL0 PORTNUM2PIN(PB, 19)
#define COL1 PORTNUM2PIN(PC, 17)
#define COL2 PORTNUM2PIN(PB, 18)

#define BUFFER_SIZE 10

/**
 * Structure which defines a key in a keyboard
 */
typedef struct{
    int pin;				// Phisical pin where the key is connected.
    char charCode;			// Character the key represents.
    bool pressed;			// Stable state (debounced).
    bool curState;          // Unstable state.
    bool lastState;         // Previous state.
    uint8_t clickCount;     // Number of button clicks within multiclickTime.
    uint64_t lastBounceTime;// The last time the pin was toggled.
}Key;

/////////////////////////////////////////////////////////////////////////////////
//                   Local variable definitions ('static')                     //
/////////////////////////////////////////////////////////////////////////////////

static Key keys[NROWS][NCOLS];

NEW_CIRCULAR_BUFFER(buffer,BUFFER_SIZE,sizeof(KeyboardEvent));

/////////////////////////////////////////////////////////////////////////////////
//                   Local function prototypes ('static')                      //
/////////////////////////////////////////////////////////////////////////////////

/**
 *	Interrupt subroutine called to multiplex the keyboard.
 */
static void keyboardPISR(void);

/**
 *	Function to update key variables and detect states.
 */
static void updateKey(Key* key);


/////////////////////////////////////////////////////////////////////////////////

void initKeyboard(void)
{
	int COLS[] = {COL0,COL1,COL2};

	char kbChars[NROWS][NCOLS] = {{'1','2','3'},
								  {'4','5','6'},
								  {'7','8','9'},
	 							  {'*','0','#'}};
	for(int j=0;j<NCOLS; j++)
	{
		// Set pins as inputs
		pinMode(COLS[j],INPUT_PULLDOWN);

		for(int i=0; i<NROWS; i++)
		{
			keys[i][j].pin = COLS[j];
			keys[i][j].charCode = kbChars[i][j];
		}
	}

	sysTickAddCallback(&keyboardPISR,(float)(1/(float)MUX_FREQUENCY));
}


static void keyboardPISR(void)
{
	// Active row
	int r = getActivePin();

	// Read columns
	for(int c=0; c<NCOLS; c++)
		updateKey(&keys[r][c]);
}

KeyboardEvent getKeyboardEvent()
{
	KeyboardEvent ev;
	if(pop(&buffer,&ev)==false)
		ev.type=KB_NO_EVT;
	return ev;
}

char getchar()
{
	KeyboardEvent ev;
	// Ignore all KB_KEY_UP events
	do{
		if(pop(&buffer,&ev)==false) // QUE DEVOLVER SI NO HAY NADA EN EL BUFFER?
			break;
	}
	while(ev.type!=KB_KEY_DOWN);

	return ev.charCode;
}

bool keyboardHit()
{
	return !isEmpty(&buffer);
}

void keyboardFlush()
{
	flush(&buffer);
}

uint8_t keyboardCount()
{
	return numel(&buffer);
}

/*
		uint8_t currStatus = digitalRead(COLS[c]);
		if(currStatus != kbStatus[r][c])
		{
			KeyboardEvent ev = {.charCode = kbChars[r][c],
						.row=r ,
						.col=c};

			if(currStatus==PRESSED)
				ev.type=KB_KEY_DOWN;
			else
				ev.type=KB_KEY_UP;

			push(&buffer,&ev);
			kbStatus[r][c] = currStatus;



			KB_KEY_UP,
				KB_KEY_DOWN,
				KB_KEY_LONG_PRESS,
				KB_KEY_SINGLE_PRESS,
				KB_KEY_MULTI_PRESS,
				KB_NO_EVT}


				*/
static void updateKey(Key* key)
{
	static KeyboardEvent ev;

	// Get current time.
	uint64_t now = millis();

	// Current button state.
	key->curState = digitalRead(key->pin);

	// If the keys are active low, invert the logic.
	if (ACTIVE_LOW) key->curState = ! key->curState;

  	// If the key changed its state, reset the bounce timer.
	if (key->curState != key->lastState)
		key->lastBounceTime = now;

	// Debounce the key
	if (now - key->lastBounceTime > DEBOUNCE_TIME && key->curState != key->pressed)
	{
		key->pressed = key->curState;
		ev.charCode = key->charCode;
		ev.clicks = 0;
		if (key->pressed)
		{
			ev.type = KB_KEY_DOWN;
			key->clickCount++;
		}
		else
			ev.type = KB_KEY_UP;
		push(&buffer,&ev);
	}

    // If the button released state is stable, report nr of clicks and start new cycle
	if (!key->pressed && (now - key->lastBounceTime) > MULTI_PRESS_TIME)
	{
		ev.charCode = key->charCode;
		ev.clicks = key->clickCount;
		switch(key->clickCount)
		{

		case 1:
			ev.type = KB_KEY_SINGLE_PRESS;
			break;
		case 2:
			ev.type = KB_KEY_MULTI_PRESS;
			break;
		}
		push(&buffer,&ev);
		key->clickCount = 0;
	}

	// Check for "long click"
	if (key->pressed && (now - key->lastBounceTime > LONG_PRESS_TIME))
	{
		ev.charCode = key->charCode;
		ev.clicks = key->clickCount;
		ev.type = KB_KEY_LONG_PRESS;
		push(&buffer,&ev);
		key->clickCount = 0;
	}

	key->lastState = key->curState;
}
