/////////////////////////////////////////////////////////////////////////////////
//                             Included header files                           //
/////////////////////////////////////////////////////////////////////////////////
#include "Multiplexer.h"
#include "GPIO.h"
#include "SysTick.h"


/////////////////////////////////////////////////////////////////////////////////
//                   Local variable definitions ('static')                     //
/////////////////////////////////////////////////////////////////////////////////

static const int N = 4; // Number of pins
static const int pins[] = { PORTNUM2PIN(PC,8), /* Pins being multiplexed */
							PORTNUM2PIN(PA,1),
							PORTNUM2PIN(PC,1),
							PORTNUM2PIN(PB,9)
						   };
static const int ON = 0; // Define active high/low pins
static const int OFF = 1;
static int activePin;

/////////////////////////////////////////////////////////////////////////////////
//                   Local function prototypes ('static')                      //
/////////////////////////////////////////////////////////////////////////////////
static void multiplexerPISR(void);

/////////////////////////////////////////////////////////////////////////////////
//                         Global functions definition                         //
/////////////////////////////////////////////////////////////////////////////////

void initMultiplexer()
{
	// Set all pins as outputs
	for(int i=0; i<N; i++)
	{
		pinMode(pins[i],OUTPUT);
		digitalWrite(pins[i],OFF);
	}
	// And set pin 0 as active
	digitalWrite(pins[0],ON);
	activePin=0;

	// Register the periodic callback
	sysTickAddCallback(&multiplexerPISR,(1/(float)MUX_FREQUENCY));
}

// Interrupt subroutine called to switch multiplexer
static void multiplexerPISR(void)
{
	digitalToggle(pins[activePin]);
	activePin = (activePin+1)%N;
	digitalToggle(pins[activePin]);
}

int getActivePin(void)
{
	return activePin;
}
