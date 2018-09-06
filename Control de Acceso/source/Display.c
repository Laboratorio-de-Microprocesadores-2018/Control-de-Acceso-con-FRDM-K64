/////////////////////////////////////////////////////////////////////////////////
//                              CONTROL DE ACCESO                              //
//          Grupo 3 - Laboratorio de Microprocesadores - ITBA - 2018           //
//	                                                                           //
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//                             Included header files                           //
/////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include "Display.h"
#include "Multiplexer.h"
#include "SysTick.h"
#include "GPIO.h"




/////////////////////////////////////////////////////////////////////////////////
//                       Constants and macro definitions                       //
/////////////////////////////////////////////////////////////////////////////////
#define DATA_LENGTH					10
#define DISPLAY_NUM					4
#define DISLAY_PIN_NUM				7		// Number of pins
#define OFF_DISP_PIN				0			// Define active high/low pins
#define ON_DISP_PIN					1
#define DISP_FREQUENCY				(float)(MUX_FREQUENCY*100)
#define MAXBRIGHT					((float)DISP_FREQUENCY)/((float)MUX_FREQUENCY)
#define MINBRIGHT					MAXBRIGHT/100




/////////////////////////////////////////////////////////////////////////////////
//                   Local variable definitions ('static')                     //
/////////////////////////////////////////////////////////////////////////////////

static const int displayPins[] = {PORTNUM2PIN(PC,2),PORTNUM2PIN(PC,0),PORTNUM2PIN(PA,2),PORTNUM2PIN(PC,5), PORTNUM2PIN(PC,3), PORTNUM2PIN(PC,7), PORTNUM2PIN(PB,23)};
static const int dotPin = PORTNUM2PIN(PC,9);
static const uint8_t numCode[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};
static char data[DATA_LENGTH];
static int muxActivePin;
static int dataIndex; // Index
static int dataCursor;
static int brightLevel;
static int brightCount;
static int callCount;
static int pinCall;





/////////////////////////////////////////////////////////////////////////////////
//                   Local function prototypes ('static')                      //
/////////////////////////////////////////////////////////////////////////////////
/**
 * @brief This function will write the pins of the 7 segment display
 *
 * @param param1 Description of the first parameter of the function.
 * @param param2 The second one, which follows @p param1.
 * @return Describe what the function returns.
 * @see Box_The_Second_Function
 * @see Box_The_Last_One
 * @see http://website/
 * @note Something to note.
 * @warning Warning.
 */
static void write7SegDisplay(uint8_t c);
// Interrupt subroutine called to advance multiplexer
static void displayPISR(void);


void initDisplay(void)
{
	brightLevel = MAXBRIGHT;
	brightCount = brightLevel;
	muxActivePin = 4;
	for(int i=0; i < 7; i++)
	{
		pinMode(displayPins[i],OUTPUT);
		digitalWrite(displayPins[i],OFF_DISP_PIN);
	}
	// Register the periodic callback
	sysTickAddCallback(&displayPISR,(float)(1/DISP_FREQUENCY));

}


// Multiplexer initialization

// Interrupt subroutine called to switch multiplexer
void displayPISR(void)
{
	static uint8_t c;
	static int newActivePin;
	newActivePin = getActivePin();

	if(muxActivePin == newActivePin)
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
			c = numCode[data[dataIndex + muxActivePin] - '0'];
		}else
		{
			c = 0x00;
		}
		write7SegDisplay(c);
		callCount ++;

		/*
		if(pinCall < DISPLAY_NUM)
			pinCall ++;
		else
		{
			pinCall = 0;
			callCount ++;
		}
		*/
	}
	/*
	if(callCount < brightCount)
	{

	}else
	{
		c = 0x00;
		write7SegDisplay(c);
		if(pinCall < DISPLAY_NUM)
			pinCall ++;
		else
		{
			pinCall = 0;
			callCount  = 0;
		}

	}
	*/

}


/*******************************************************************************
 * SERVICES
 ******************************************************************************/
void putchar(char c)
{
	if(dataIndex < DATA_LENGTH - DISPLAY_NUM)
	{
		data[dataIndex + dataCursor] = c;
		if(dataCursor  + 1 <= DISPLAY_NUM)
		{
			dataCursor ++;
		}else
		{
			dataIndex ++;
		}
	}

}

void erase(void)
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

void brightUp(void)
{
	//static float intenseFraction;
	if(brightLevel < MAXBRIGHT)
	{
		brightLevel += 5;
		//intenseFraction = ((float) IO)^(((brightLevel)/((float) MAXBRIGHT))-1);
		brightCount = brightLevel;

	}
}
void brightDown(void)
{
	//static float intenseFraction;
	if(brightLevel > MINBRIGHT)
	{
		brightLevel -= 5;
		//intenseFraction = ((float) IO)^(((brightLevel)/((float) MAXBRIGHT))-1);
		brightCount = brightLevel;
		if(brightCount <= 0)
		{
			brightCount = 1;
		}

	}
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

