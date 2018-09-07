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
static char data[DATA_LENGTH];		//	Array of codes to be printed in the displays
static int muxActivePin;			//	Number of display being used, it will be defined by the mux
static int dataIndex;				//	Last number of the array data, to be displayed
static int dataCursor;				//	Pointer to the next position to be written in the data array
static int brightLevel;				//	bright level that emulates the duty cycle of the display
static int brightCount;				//	Counter in order to control the duty cycle of the display
static int callCount;				//	Counter in order to control the duty cycle of the display






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


/////////////////////////////////////////////////////////////////////////////////
//                   Local function definitions			                       //
/////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Interrupt subroutine called to update the state of the displays.
 * The displays will be ON, one at a time. The one being updated will be defined by the active pin set by the MUX.
 * Then, in @data, it will be stored the array of characters to be written. In order to know which character corresponds
 * to each display, the variables @dataIndex and @dataCursor will be used to reference the correct characters,inside
 * @data. @dataIndex will always refer to the number of char inside @data, that must be printed to the first display.
 * Then, the chars to be printed in the other displays will re referred by @daataIndex + @muxActivePin.
 */
void displayPISR(void)
{
	static uint8_t c;					/**<	The eventual code @c to be written to the active is display is declared*/
	static int newActivePin;
	newActivePin = getActivePin();		/**<	The value of the active display is updated. As the frequency with which the displayCallback is called is 'k' times greater than the frequency of the muxCallback, the callback will be called 'k' times before this value actually changes*/

	/*	If the callBack its being called for first time (callCount != 0), or the active display is the same as the last time, callCount is incremented.
	 * This way, the percentage of the time that has passed with the display ON, it will be proportional to callCount
	 * Then the duty cycle of the display will be set by how many times the callback is called.
	 * If i haven't reached the duty cycle required for the given brightLevel, then i keep the pins as they where written when the activePin changed*/
	if((muxActivePin == newActivePin) & (callCount != 0))
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
		muxActivePin = newActivePin;	/**<	The active pin is updated*/
		if((muxActivePin >= 0) & (muxActivePin < dataCursor))
		{
			c = data[dataIndex + muxActivePin];		/**<	The char code to be written is referenced as it was explained in the @brief*/
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

/**
 * @brief Adds the char @c to the array @data to the position "referenced" by (@dataIndex + @dataCursor). Then it will be printed to the corresponding display.
 */
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
/*			case '.' :
*				data[dataIndex + dataCursor] = ;
*				break;
*/
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

/**
 * @brief Erases the last char from @data
 */
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

/**
 * @brief Clears all the @data array
 */
void dispClear(void)
{
	for(int i = 0; i < dataIndex + dataCursor;  i++)
	{
		data[i] = 0;
	}
	dataIndex = 0;
	dataCursor = 0;
}

/**
 * @brief Clears all the @data array, and adds a certain message to it, In order to eventually display it on the display.
 */
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

/**
 * @brief Does the same as dispMessage, but only adds the message 'Err' and the type of error according to @c
 *
 * @param c the type of error to be displayed
 */
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
	{
		dispPutchar(numChars[i]);
	}

}

/**
 * @brief Increments the brightLevel of the displays. In the end it will incremente the brightCount so the Displays are turned ON during a longer period of time
 */
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

/**
 * @brief Same concept as brightUp, but it takes the brightLevel down.
 */
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

/**
 * @brief This gets the current brightness level of the displays
 */
int getBrightnessLevel(void)
{
	return brightLevel;
}
