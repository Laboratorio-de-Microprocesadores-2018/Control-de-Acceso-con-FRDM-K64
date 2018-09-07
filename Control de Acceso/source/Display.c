/////////////////////////////////////////////////////////////////////////////////
//                              CONTROL DE ACCESO                              //
//          Grupo 3 - Laboratorio de Microprocesadores - ITBA - 2018           //
//	                                                                           //
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//                             Included header files                           //
/////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>			//	For certain variables
#include <stdlib.h>			//	To convert int to char
#include "Display.h"		//	This is the header of the file
#include "Multiplexer.h"	//	This is used for the communication with the mux
#include "SysTick.h"		//
#include "GPIO.h"

/////////////////////////////////////////////////////////////////////////////////
//                       Constants and macro definitions                       //
/////////////////////////////////////////////////////////////////////////////////
#define DATA_LENGTH					10		//	Maximum number of characters to be displayed

#define DISPLAY_NUM					4		//	Number of displays used in the board (this could be in bord.h)
#define DISLAY_PIN_NUM				7		//	Number of pins in the 7segment display

#define OFF_DISP_PIN				0		//	Define active high/low pins
#define ON_DISP_PIN					1


#define DISP_FREQUENCY				(float)(MUX_FREQUENCY*10)						//	Frequency for the dispPISR callback (It has to be faster than the one of th MUX, in order to change the brightness)
#define MAXBRIGHT					((float)DISP_FREQUENCY)/((float)MUX_FREQUENCY)	//	Maximum amount of brightness, this will emulate the duty cicle of the timne the display is on
#define MINBRIGHT					MAXBRIGHT/100									//	Following the line of thought of MAXBRIGHT, this will be the minimum duty cicle of the time the display is on

//	The next enum will have the codes that must be printed to the display pins, in order to display certain character
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
	DISP_t		= 0x78
}dispCodes;



/////////////////////////////////////////////////////////////////////////////////
//                   Local variable definitions ('static')                     //
/////////////////////////////////////////////////////////////////////////////////

static const int displayPins[] = {PORTNUM2PIN(PC,2),PORTNUM2PIN(PC,0),PORTNUM2PIN(PA,2),PORTNUM2PIN(PC,5), PORTNUM2PIN(PC,3), PORTNUM2PIN(PC,7), PORTNUM2PIN(PB,23)};	// Definition of the pins to be used, in order to write the display
static const int dotPin = PORTNUM2PIN(PC,9);		// Definition of the "dot" of the 7 segment display pin
//static const uint8_t numCode[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};
static char data[DATA_LENGTH];		//	Array of codes to be printed in the displays
static int muxActivePin;			//	This will be the number of display is being used, it will be defined by the mux
static int dataIndex;				//	Last number of the array data, to be displayed
static int dataCursor;				//	Pointer to the next position to be written in the data array
static int brightLevel;				//	bright level that emulates the duty cicle of the display
static int brightCount;				//	Counter in order to control the duty cicle of the display
static int callCount;				//	Counter in order to control the duty cicle of the display
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
	brightLevel = MAXBRIGHT;										//The initial brit¿ghtness is the maximum possible
	brightCount = brightLevel;										//The time count of the futy cilce is equal to the bright level

	/*	Inizialization of the pins connected to the display, and i set them as OFF in order to see nothing in the display*/
	for(int i=0; i < 7; i++)
	{
		pinMode(displayPins[i],OUTPUT);
		digitalWrite(displayPins[i],OFF_DISP_PIN);
	}
	/*	The callback of the display is added to the sysTick callback list*/
	sysTickAddCallback(&displayPISR,(float)(1/DISP_FREQUENCY));

}

/**
 * @brief Interrupt subroutine called to update the state of the displays.
 * Basically, depending on the pin the mux has active, it will write on the corresponding display of the mentioned pin, what the array @data has stored for given display
 */
void displayPISR(void)
{
	static uint8_t c;
	static int newActivePin;
	newActivePin = getActivePin();

	if((muxActivePin == newActivePin) || (callCount == 0))
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
		muxActivePin = getActivePin();
		if((muxActivePin >= 0) & (muxActivePin < dataCursor))
		{
			//c = numCode[data[dataIndex + muxActivePin]];
			c = data[dataIndex + muxActivePin];
		}else
		{
			c = 0x00;
		}
		write7SegDisplay(c);
		callCount ++;
	}

}


/*******************************************************************************
 * SERVICES
 ******************************************************************************/
void dispPutchar(char c)
{
	if(dataIndex < DATA_LENGTH - DISPLAY_NUM)
	{
		switch(c)
		{
			case '0' :
				data[dataIndex + dataCursor] = DISP_ZERO;
				break;
			case '1' :
				data[dataIndex + dataCursor] = DISP_ONE;
				break;
			case '2' :
				data[dataIndex + dataCursor] = DISP_TWO;
				break;
			case '3' :
				data[dataIndex + dataCursor] = DISP_THREE;
				break;
			case '4' :
				data[dataIndex + dataCursor] = DISP_FOUR;
				break;
			case '5' :
				data[dataIndex + dataCursor] = DISP_FIVE;
				break;
			case '6' :
				data[dataIndex + dataCursor] = DISP_SIX;
				break;
			case '7' :
				data[dataIndex + dataCursor] = DISP_SEVEN;
				break;
			case '8' :
				data[dataIndex + dataCursor] = DISP_EIGHT;
				break;
			case '9' :
				data[dataIndex + dataCursor] = DISP_NINE;
				break;
			case '-' :
				data[dataIndex + dataCursor] = DISP_DASH;
				break;
			case 'A' :
				data[dataIndex + dataCursor] = DISP_A;
				break;
			case 'E' :
				data[dataIndex + dataCursor] = DISP_E;
				break;
			case 'L' :
				data[dataIndex + dataCursor] = DISP_L;
				break;
			case 'P' :
				data[dataIndex + dataCursor] = DISP_P;
				break;
			case 'n' :
				data[dataIndex + dataCursor] = DISP_n;
				break;
			case 'r' :
				data[dataIndex + dataCursor] = DISP_r;
				break;
			case 'd' :
				data[dataIndex + dataCursor] = DISP_d;
				break;
			case 'o' :
				data[dataIndex + dataCursor] = DISP_o;
				break;
			case 'u' :
				data[dataIndex + dataCursor] = DISP_u;
				break;
			case 't' :
				data[dataIndex + dataCursor] = DISP_t;
				break;
//			case '.' :
//				data[dataIndex + dataCursor] = ;
				break;
		}
		if(dataCursor < DISPLAY_NUM)
		{
			dataCursor ++;
		}else
		{
			dataIndex ++;
		}
	}

}

void dispErase(void)
{
	if(dataIndex > 0)
	{
		dataIndex --;
		data[dataIndex + dataCursor] = 0x00;
	}else if(dataCursor > 0 )
	{
		dataCursor --;
		data[dataCursor] = 0x00;
	}

}

void dispClear(void)
{
	for(int i = 0; i < dataIndex + dataCursor;  i++)
	{
		data[i] = 0;
	}
	dataIndex = 0;
	dataCursor = 0;
}

void dispMessage(Message msg){
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
	{
		dispPutchar(numChars[i]);
	}

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
/*******************************************************************************
 * ADDITIONAL FUNCTIONS
 ******************************************************************************/
void write7SegDisplay(uint8_t c)
{

	for(int i = 0; i < DISLAY_PIN_NUM; i++)
	{
		digitalWrite(displayPins[i], (((c) >> i) & 0x01));
	}

}

