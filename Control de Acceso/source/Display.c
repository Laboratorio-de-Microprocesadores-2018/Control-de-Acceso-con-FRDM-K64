/////////////////////////////////////////////////////////////////////////////////
//                              CONTROL DE ACCESO                              //
//          Grupo 3 - Laboratorio de Microprocesadores - ITBA - 2018           //
//	                                                                           //
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//                             Included header files                           //
/////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdlib.h>
#include "Display.h"
#include "Multiplexer.h"
#include "SysTick.h"
#include "GPIO.h"

/////////////////////////////////////////////////////////////////////////////////
//                       Constants and macro definitions                       //
/////////////////////////////////////////////////////////////////////////////////

/**  Maximum number of characters to be displayed. */
#define DATA_LENGTH			20

/** Number of displays used in the board. */
#define DISPLAY_NUM			4

/** Define active high/low pins. */
#define OFF_DISP_PIN		0
#define ON_DISP_PIN			1

/** Percentage step when increasing/decreasing brightness. */
#define BRIGHTNESS_STEP     5

/** Number of steps from 0% to 100%. */
#define NUMBER_OF_STEPS     (100/(float)BRIGHTNESS_STEP)

/**	Frequency for the dispPISR callback (It has to be faster than the one of th MUX, in order to change the brightness) */
#define DISP_FREQUENCY		(float)(MUX_FREQUENCY * NUMBER_OF_STEPS)

/**	Maximum amount of brightness, this will emulate the duty cicle of the timne the display is on */
#define MAXBRIGHT			100
/**	Following the line of thought of MAXBRIGHT, this will be the minimum duty cicle of the time the display is on */
#define MINBRIGHT			1

/** Define GPIO pins. */
enum{
	SEG_A = PORTNUM2PIN(PC,5),
	SEG_B = PORTNUM2PIN(PC,3),
	SEG_C = PORTNUM2PIN(PC,7),
	SEG_D = PORTNUM2PIN(PC,2),
	SEG_E = PORTNUM2PIN(PC,0),
	SEG_F = PORTNUM2PIN(PA,2),
	SEG_G = PORTNUM2PIN(PB,23),
	DP    = PORTNUM2PIN(PC,9)
};

#define DOOR_LED PORTNUM2PIN(PC,16)

//** Define the binary values for the display pins, in order to display certain character */
typedef enum{
	DISP_ZERO 	= 0x3F,
	DISP_ONE 	= 0x06,
	DISP_TWO 	= 0x5B,
	DISP_THREE 	= 0x4F,
	DISP_FOUR 	= 0x66,
	DISP_FIVE 	= 0x6D,
	DISP_SIX 	= 0x7D,
	DISP_SEVEN 	= 0x07,
	DISP_EIGHT 	= 0x7F,
	DISP_NINE 	= 0x67,
	DISP_DASH 	= 0x40,
	DISP_A		= 0x77,
	DISP_E		= 0x79,
	DISP_L		= 0x38,
	DISP_P		= 0x73,
	DISP_n		= 0x54,
	DISP_r		= 0x50,
	DISP_d      = 0x5E,
	DISP_o      = 0x5C,
	DISP_u      = 0x1C,
	DISP_t		= 0x78,
	DISP_b		= 0x7C,
	DISP_i		= 0x10,
	DISP_dot	= 0x80
}dispCodes;



/////////////////////////////////////////////////////////////////////////////////
//                   Local variable definitions ('static')                     //
/////////////////////////////////////////////////////////////////////////////////

<<<<<<< HEAD
<<<<<<< HEAD
static const int displayPins[] = {PORTNUM2PIN(PC,2),PORTNUM2PIN(PC,0),PORTNUM2PIN(PA,2),PORTNUM2PIN(PC,5), PORTNUM2PIN(PC,3), PORTNUM2PIN(PC,7), PORTNUM2PIN(PB,23)};	// Definition of the pins to be used, in order to write the display
static const int dotPin = PORTNUM2PIN(PC,9);		// Definition of the "dot" of the 7 segment display pin
static char data[DATA_LENGTH];		//	Array of codes to be printed in the displays
static int muxActivePin;			//	Number of display being used, it will be defined by the mux
static int dataIndex;				//	Last number of the array data, to be displayed
static int dataCursor;				//	Pointer to the next position to be written in the data array
static int brightLevel;				//	bright level that emulates the duty cycle of the display
static int brightCount;				//	Counter in order to control the duty cycle of the display
static int callCount;				//	Counter in order to control the duty cycle of the display

=======
=======
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489
/** Array to iterate the pins */
static const int displayPins[] = {SEG_D,SEG_E,SEG_F,SEG_A,SEG_B,SEG_C,SEG_G,DP};//{PORTNUM2PIN(PC,2),PORTNUM2PIN(PC,0),PORTNUM2PIN(PA,2),PORTNUM2PIN(PC,5), PORTNUM2PIN(PC,3), PORTNUM2PIN(PC,7), PORTNUM2PIN(PB,23)};

/**	Array of codes to be printed in the displays. */
static char data[DATA_LENGTH];

/**	This will be the number of display is being used, it will be defined by the mux */
static int activeDisplay;

/**	Last number of the array data, to be displayed. */
static int index;

/**	Pointer to the next position to be written in the data array. */
static int cursor;

/**	bright level that emulates the duty cicle of the display. */
static int brightLevel;

/**	Counter in order to control the duty cicle of the display. */
static int brightCount;

/**	Counter in order to control the duty cicle of the display. */
static int callCount;
//static int pinCall;
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489





/////////////////////////////////////////////////////////////////////////////////
//                   Local function prototypes ('static')                      //
/////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Writes @c to the pins of the display
 *
 * @param c The code to be printed in the pins of the display, as each display has 8 pins, is of type uint8_t
 */
static void write7SegDisplay(uint8_t c);

/**
 * @brief Interrupt subroutine called to update the state of the displays
 */
static void displayPISR(void);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489
/*
 *
 */
static void toggleRedLed();

/**
 * @brief Initializes all the variables for the proper functionality of the dispalay
 */
void initDisplay(void)
{
	brightLevel = MAXBRIGHT;	//The initial brit¿ghtness is the maximum possible
	brightCount = brightLevel;	//The time count of the futy cilce is equal to the bright level

	/*	Inizialization of the pins connected to the display, and i set them as OFF
	 *  in order to see nothing in the display*/
	for(int i=0; i < 7; i++)
	{
		pinMode(displayPins[i],OUTPUT);
		digitalWrite(displayPins[i],OFF_DISP_PIN);
	}

	/** RGB led of kinetis board*/
	pinMode(PIN_LED_GREEN,OUTPUT);
	pinMode(PIN_LED_BLUE,OUTPUT);
	pinMode(PIN_LED_RED,OUTPUT);
	pinMode(DOOR_LED,OUTPUT);

	digitalWrite(PIN_LED_GREEN,HIGH);
	digitalWrite(PIN_LED_BLUE,HIGH);
	digitalWrite(PIN_LED_RED,HIGH);
	digitalWrite(DOOR_LED,0);

	/*	The callback of the display is added to the sysTick callback list*/
	sysTickAddCallback(&displayPISR,(float)(1/DISP_FREQUENCY));

}
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489

/////////////////////////////////////////////////////////////////////////////////
//                   Local function definitions			                       //
/////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Interrupt subroutine called to update the state of the displays.
<<<<<<< HEAD
<<<<<<< HEAD
 * The displays will be ON, one at a time. The one being updated will be defined by the active pin set by the MUX.
 * Then, in @data, it will be stored the array of characters to be written. In order to know which character corresponds
 * to each display, the variables @dataIndex and @dataCursor will be used to reference the correct characters,inside
 * @data. @dataIndex will always refer to the number of char inside @data, that must be printed to the first display.
 * Then, the chars to be printed in the other displays will re referred by @daataIndex + @muxActivePin.
=======
 * Basically, depending on the pin the mux has active, it will write on the corresponding
 * display of the mentioned pin, what the array @data has stored for given display
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489
=======
 * Basically, depending on the pin the mux has active, it will write on the corresponding
 * display of the mentioned pin, what the array @data has stored for given display
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489
 */
void displayPISR(void)
{
	static uint8_t c;					/**<	The eventual code @c to be written to the active is display is declared*/
	static int newActivePin;
<<<<<<< HEAD
	newActivePin = getActivePin();		/**<	The value of the active display is updated. As the frequency with which the displayCallback is called is 'k' times greater than the frequency of the muxCallback, the callback will be called 'k' times before this value actually changes*/

<<<<<<< HEAD
	/*	If the callBack its being called for first time (callCount != 0), or the active display is the same as the last time, callCount is incremented.
	 * This way, the percentage of the time that has passed with the display ON, it will be proportional to callCount
	 * Then the duty cycle of the display will be set by how many times the callback is called.
	 * If i haven't reached the duty cycle required for the given brightLevel, then i keep the pins as they where written when the activePin changed*/
	if((muxActivePin == newActivePin) & (callCount != 0))
=======
	if((activeDisplay == newActivePin) || (callCount == 0))
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489
=======
	newActivePin = getActivePin();

	if((activeDisplay == newActivePin) || (callCount == 0))
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489
	{
		if(callCount < brightCount)			/**<	While the callCount is less than the count needed for the brightness desired*/
		{
			callCount ++;					/**<	The callCount keeps incrementing*/
		}else if(callCount == brightCount)	/**<	When the bright level desired is reached*/
		{
			c = 0x00;						/**<	The display is turned OFF, writing ceroes on its corresponding pins*/
			write7SegDisplay(c);
			callCount ++;					/**<	Finally the callCount is incremented one more time so this sequence is not called again in a useless way*/
		}


	}else
	{
		/*	In the case the active pin is changed, or the callBack is called for first time, the callCount is reseted and the
		 *  pins of the given display are written with the corresponding char code, given by @data.
		 **/
		callCount = 0;
<<<<<<< HEAD
<<<<<<< HEAD
		muxActivePin = newActivePin;	/**<	The active pin is updated*/
		if((muxActivePin >= 0) & (muxActivePin < dataCursor))
		{
			c = data[dataIndex + muxActivePin];		/**<	The char code to be written is referenced as it was explained in the @brief*/
=======
		activeDisplay = getActivePin();
		if((activeDisplay >= 0) & (activeDisplay < cursor))
		{
			//c = numCode[data[index + activeDisplay]];
			c = data[index + activeDisplay];
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489
=======
		activeDisplay = getActivePin();
		if((activeDisplay >= 0) & (activeDisplay < cursor))
		{
			//c = numCode[data[index + activeDisplay]];
			c = data[index + activeDisplay];
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489
		}else
		{
			c = 0x00;								/**<	If something is not okay with the active pin, nothing is displayed*/
		}
		write7SegDisplay(c);						/**<	Finally the char code is written and callCount incremented*/
		callCount ++;
	}

}

/**
 * @brief Initializes all the variables for the proper functionality of the display
 */
void write7SegDisplay(uint8_t c)
{

	for(int i = 0; i < DISLAY_PIN_NUM; i++)
	{
		digitalWrite(displayPins[i], (((c) >> i) & 0x01));
	}

}

<<<<<<< HEAD

/////////////////////////////////////////////////////////////////////////////////
//                   Global function definitions (SERVICES)                    //
/////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Initializes all the variables for the proper functionality of the display
 */
void initDisplay(void)
{
	brightLevel = MAXBRIGHT;										//The initial brightness is the maximum possible
	brightCount = brightLevel;										//The time count of the duty cycle is equal to the bright level

	/*	Initialization of the pins connected to the display, and i set them as OFF in order to see nothing in the display*/
	for(int i=0; i < 7; i++)
	{
		pinMode(displayPins[i],OUTPUT);
		digitalWrite(displayPins[i],OFF_DISP_PIN);
	}
	/*	The callback of the display is added to the sysTick callback list*/
	sysTickAddCallback(&displayPISR,(float)(1/DISP_FREQUENCY));

}

<<<<<<< HEAD
/**
 * @brief Adds the char @c to the array @data to the position "referenced" by (@dataIndex + @dataCursor). Then it will be printed to the corresponding display.
 */
=======
/////////////////////////////////////////////////////////////////////////////////
//                   		     	Services                  				   //
/////////////////////////////////////////////////////////////////////////////////
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489
=======
/////////////////////////////////////////////////////////////////////////////////
//                   		     	Services                  				   //
/////////////////////////////////////////////////////////////////////////////////
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489
void dispPutchar(char c)
{
	if(index < DATA_LENGTH - DISPLAY_NUM)
	{
		switch(c)
		{
			case '0' :
				data[index + cursor] = DISP_ZERO;
				break;
			case '1' :
				data[index + cursor] = DISP_ONE;
				break;
			case '2' :
				data[index + cursor] = DISP_TWO;
				break;
			case '3' :
				data[index + cursor] = DISP_THREE;
				break;
			case '4' :
				data[index + cursor] = DISP_FOUR;
				break;
			case '5' :
				data[index + cursor] = DISP_FIVE;
				break;
			case '6' :
				data[index + cursor] = DISP_SIX;
				break;
			case '7' :
				data[index + cursor] = DISP_SEVEN;
				break;
			case '8' :
				data[index + cursor] = DISP_EIGHT;
				break;
			case '9' :
				data[index + cursor] = DISP_NINE;
				break;
			case '-' :
				data[index + cursor] = DISP_DASH;
				break;
			case 'A' :
				data[index + cursor] = DISP_A;
				break;
			case 'E' :
				data[index + cursor] = DISP_E;
				break;
			case 'L' :
				data[index + cursor] = DISP_L;
				break;
			case 'P' :
				data[index + cursor] = DISP_P;
				break;
			case 'n' :
				data[index + cursor] = DISP_n;
				break;
			case 'r' :
				data[index + cursor] = DISP_r;
				break;
			case 'd' :
				data[index + cursor] = DISP_d;
				break;
			case 'o' :
				data[index + cursor] = DISP_o;
				break;
			case 'u' :
				data[index + cursor] = DISP_u;
				break;
			case 't' :
				data[index + cursor] = DISP_t;
<<<<<<< HEAD
				break;
			case 'b' :
				data[index + cursor] = DISP_b;
				break;
<<<<<<< HEAD
/*			case '.' :
*				data[dataIndex + dataCursor] = ;
*				break;
*/
=======
			case 'i' :
				data[index + cursor] = DISP_i;
				break;
=======
				break;
			case 'b' :
				data[index + cursor] = DISP_b;
				break;
			case 'i' :
				data[index + cursor] = DISP_i;
				break;
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489
			case '.' :
				data[index + cursor-1] |= DISP_dot ;
				break;
			default:
				data[index + cursor] = DISP_DASH;
<<<<<<< HEAD
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489
=======
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489
		}

		if(c!='.')
		{
			if(cursor < DISPLAY_NUM)
				cursor ++;
			else
				index ++;
		}
	}

}

/**
 * @brief Erases the last char from @data
 */
void dispErase(void)
{
	/// If in the current position there is a dot, remove it.
	/// Else remove the hole character.
	if((data[index + cursor]>>8) & 0x01)
		data[index + cursor] &= ~DISP_dot;
	else
		data[index + cursor] = 0x00;

	/// And update the index
	if(index > 0)
		index --;
	else if(cursor > 0 )
		cursor --;
}

/**
 * @brief Clears all the @data array
 */
void dispClear(void)
{
	for(int i = 0; i < index + cursor;  i++)
		data[i] = 0;
	index = 0;
	cursor = 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
/**
 * @brief Clears all the @data array, and adds a certain message to it, In order to eventually display it on the display.
 */
void dispMessage(Message msg){
=======
void dispMessage(Message msg)
{
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489
=======
void dispMessage(Message msg)
{
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489
	dispClear();
	switch(msg){
		case PASS:
			dispPutchar('P');dispPutchar('A');dispPutchar('5');dispPutchar('5');
			break;
		case ADD:
			dispPutchar('A');dispPutchar('d');dispPutchar('d');
			break;
		case DEL:
			dispPutchar('0');dispPutchar('E');dispPutchar('L');
			break;
		case OPEN:
			dispPutchar('0');dispPutchar('P');dispPutchar('E');dispPutchar('n');
			break;
		case ERR:
			dispPutchar('E');dispPutchar('r');dispPutchar('r');
			break;
		case EDIT:
			dispPutchar('E');dispPutchar('d');dispPutchar('1');dispPutchar('t');
			break;
		case SUDO:
			dispPutchar('5');dispPutchar('u');dispPutchar('d');dispPutchar('o');
			break;
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
/**
 * @brief Does the same as dispMessage, but only adds the message 'Err' and the type of error according to @c
 *
 * @param c the type of error to be displayed
 */
=======
=======
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489
void dispString(char * s)
{
	ASSERT(s!=NULL);

	while((*s)!='\0')
		dispPutchar((*s++));
}

<<<<<<< HEAD
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489
=======
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489
void dispError(char c)
{
	dispMessage(ERR);
	dispPutchar(c);
}

/**
 * @brief Same as displayMsg, but with a certai number @n
 *
 * @param n the number to be displayed. In the function it`s executed an 'itoa()' stage, in order to get the string corresponding to the sent number @n
 */
void displayNum(int n)
{
	char numChars[DISPLAY_NUM + 1];
	itoa(n,numChars,10);
	dispClear();
	for(int i = 0; numChars[i] != 0x00; i++)
		dispPutchar(numChars[i]);
}

/**
 * @brief Increments the brightLevel of the displays. In the end it will incremente the brightCount so the Displays are turned ON during a longer period of time
 */
void brightnessUp(void)
{
	//static float intenseFraction;
	if(brightLevel+BRIGHTNESS_STEP <= MAXBRIGHT)
	{
		brightLevel += BRIGHTNESS_STEP;
		//intenseFraction = ((float) IO)^(((brightLevel)/((float) MAXBRIGHT))-1);
		brightCount = brightLevel;

	}
}

<<<<<<< HEAD
<<<<<<< HEAD
/**
 * @brief Same concept as brightUp, but it takes the brightLevel down.
 */
=======
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489
=======
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489
void brightnessDown(void)
{
	//static float intenseFraction;
	if(brightLevel-BRIGHTNESS_STEP >= MINBRIGHT)
	{
		brightLevel -= BRIGHTNESS_STEP;
		//intenseFraction = ((float) IO)^(((brightLevel)/((float) MAXBRIGHT))-1);
		brightCount = brightLevel;
	}
}

/**
 * @brief This gets the current brightness level of the displays
 */
int getBrightnessLevel(void)
{
	return brightLevel;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489

void dispOpenDoor(void)
{
	digitalWrite(DOOR_LED,1);
	digitalWrite(PIN_LED_GREEN,0);
}

void dispCloseDoor(void)
{
	digitalWrite(DOOR_LED,0);
	digitalWrite(PIN_LED_GREEN,1);
}

void dispErrorLed(void)
{
	digitalWrite(PIN_LED_RED,0);
	sysTickAddDelayCall(&toggleRedLed,0.7);
}

static void toggleRedLed()
{
	digitalToggle(PIN_LED_RED);
}

static void write7SegDisplay(uint8_t c)
{
	for(int i = 0; i < 8; i++)
		digitalWrite(displayPins[i], (((c) >> i) & 0x01));
}


<<<<<<< HEAD
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489
=======
>>>>>>> edd8d4a3cfafb581cde9e20a9a6db299eae7c489
