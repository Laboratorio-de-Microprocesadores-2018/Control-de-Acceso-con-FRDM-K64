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
#define MAXBRIGHT			((float)DISP_FREQUENCY)/((float)MUX_FREQUENCY)
/**	Following the line of thought of MAXBRIGHT, this will be the minimum duty cicle of the time the display is on */
#define MINBRIGHT			MAXBRIGHT/100

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
	DISP_dot	= 0x80
}dispCodes;



/////////////////////////////////////////////////////////////////////////////////
//                   Local variable definitions ('static')                     //
/////////////////////////////////////////////////////////////////////////////////

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

	digitalWrite(PIN_LED_GREEN,HIGH);
	digitalWrite(PIN_LED_BLUE,HIGH);
	digitalWrite(PIN_LED_RED,HIGH);

	/*	The callback of the display is added to the sysTick callback list*/
	sysTickAddCallback(&displayPISR,(float)(1/DISP_FREQUENCY));

}

/**
 * @brief Interrupt subroutine called to update the state of the displays.
 * Basically, depending on the pin the mux has active, it will write on the corresponding
 * display of the mentioned pin, what the array @data has stored for given display
 */
void displayPISR(void)
{
	static uint8_t c;
	static int newActivePin;
	newActivePin = getActivePin();

	if((activeDisplay == newActivePin) || (callCount == 0))
	{
		if(callCount < brightCount)
		{
			callCount ++;
		}else
		{	if(callCount == brightCount)
			{
				c = 0x00;
				write7SegDisplay(c);
				callCount ++;
			}
		}

	}else
	{
		callCount = 0;
		activeDisplay = getActivePin();
		if((activeDisplay >= 0) & (activeDisplay < cursor))
		{
			//c = numCode[data[index + activeDisplay]];
			c = data[index + activeDisplay];
		}else
		{
			c = 0x00;
		}
		write7SegDisplay(c);
		callCount ++;
	}

}


/////////////////////////////////////////////////////////////////////////////////
//                   		     	Services                  				   //
/////////////////////////////////////////////////////////////////////////////////
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
				break;
			case '.' :
				data[index + cursor] |= DISP_dot ;
				break;
			default:
				data[index + cursor] = DISP_DASH;
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

void dispClear(void)
{
	for(int i = 0; i < index + cursor;  i++)
		data[i] = 0;
	index = 0;
	cursor = 0;
}

void dispMessage(Message msg)
{
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

void dispString(char * s)
{
	while((*s)!='\0')
		putchar((*s++));
}

void dispError(char c)
{
	dispMessage(ERR);
	dispPutchar(c);
}

void displayNum(int n)
{
	char numChars[DISPLAY_NUM + 1];
	itoa(n,numChars,10);
	dispClear();
	for(int i = 0; numChars[i] != 0x00; i++)
		dispPutchar(numChars[i]);
}

void brightnessUp(void)
{
	//static float intenseFraction;
	if(brightLevel+STEP <= MAXBRIGHT)
	{
		brightLevel += STEP;
		//intenseFraction = ((float) IO)^(((brightLevel)/((float) MAXBRIGHT))-1);
		brightCount = brightLevel;

	}
}
void brightnessDown(void)
{
	//static float intenseFraction;
	if(brightLevel-STEP >= MINBRIGHT)
	{
		brightLevel -= STEP;
		//intenseFraction = ((float) IO)^(((brightLevel)/((float) MAXBRIGHT))-1);
		brightCount = brightLevel;
	}
}

int getBrightnessLevel(void)
{
	return brightLevel;
}

static void write7SegDisplay(uint8_t c)
{
	for(int i = 0; i < 8; i++)
		digitalWrite(displayPins[i], (((c) >> i) & 0x01));
}

