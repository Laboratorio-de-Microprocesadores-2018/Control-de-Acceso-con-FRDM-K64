#include "Multiplexer.h"
#include "GPIO.h"
#include "SysTick.h"

// Number of pins
static const int N = 4;
// Pins being multiplexed
static const int pins[] = {PORTNUM2PIN(PB, 0),PORTNUM2PIN(PB, 1),PORTNUM2PIN(PB, 2),PORTNUM2PIN(PB, 3)};
// Define active high/low pins
#define ON 0
#define OFF 1

//////////////////////////   Static variables
static int activePin;

//                Funciones estaticas
// Interrupt subroutine called to advance multiplexer
static void multiplexerPISR(void);

// Multiplexer initialization
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
	sysTickAddCallback(&multiplexerPISR(),(float)(1/MUX_FREQUENCY));
}

// Interrupt subroutine called to switch multiplexer
void multiplexerPISR(void)
{
	digitalToggle(pins[activePin]);
	activePin = (activePin+1)%N;
	digitalToggle(pins[activePin]);
}

int getActivePin(void)
{
	// ES MEJOR ESTO O LEER EL PUERTO EN ESTE INSTANTE{ñ?????????
	return activePin;
}
