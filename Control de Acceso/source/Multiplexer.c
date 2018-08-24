#include "Multiplexer.h"
#include "GPIO.h"

// Constants
static const int pins[] = {PORTNUM2PIN(PB, 0),PORTNUM2PIN(PB, 1),PORTNUM2PIN(PB, 2),PORTNUM2PIN(PB, 3)};
static const int N = 4;
static const int ON = 0;
static const int OFF = 1;
// Variables
static int activePin;
static int muxFreq;
// Multiplexer initialization
void initMultiplexer(int freq)
{
	muxFreq = freq;
	for(int i=0; i<N; i++)
	{
		pinMode(pins[i],OUTPUT);
		digitalWrite(pins[i],OFF);
	}
	digitalWrite(pins[0],ON);
	activePin=0;
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
	// ES MEJOR ESTO O LEER EL PUERTO?????????
	return activePin;
}
