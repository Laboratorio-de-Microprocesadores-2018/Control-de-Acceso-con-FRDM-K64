#include "./gpio.h"
#include "hardware.h"

/**
 * @brief Configures the specified pin to behave either as an input or an output
 * @param pin the pin whose mode you wish to set (according PORTNUM2PIN)
 * @param mode INPUT, OUTPUT, INPUT_PULLUP or INPUT_PULLDOWN.
 */
void pinMode (uint8_t pin, uint8_t mode)
{
	if(pin >= 6*32)
		return;

	uint8_t i = pin/32UL; // Numero del puerto (A,B,C,D,E)
	uint8_t n = pin%32UL; // Numero del pin [0:31]
	static PORT_Type * const ports[] = PORT_BASE_PTRS;
	static GPIO_Type * const gpio[] = GPIO_BASE_PTRS;

	// Configurar el pin como GPIO
	ports[i]->PCR[n] |= PORT_PCR_MUX(1);

	switch(mode)
	{
	case INPUT:
		ports[i]->PCR[n] &= ~PORT_PCR_PE(1); // Poner en 0 el Pull Enable
		gpio[i]->PDDR &= ~(1UL<<n); // Poner en 0 el bit n del PDDR correspondiente
		break;
	case INPUT_PULLUP:
		ports[i]->PCR[n] |= PORT_PCR_PE(1); // Poner en 1 el Pull Enable
		ports[i]->PCR[n] |= PORT_PCR_PS(1); // Poner en 1 el Pull Select
		gpio[i]->PDDR &= ~(1UL<<n);
		break;
	case INPUT_PULLDOWN:
		ports[i]->PCR[n] |= PORT_PCR_PE(1); // Poner en 1 el Pull Enable
		ports[i]->PCR[n] &= ~PORT_PCR_PS(1); // Poner en 0 el Pull Select
		gpio[i]->PDDR &= ~(1UL<<n);
		break;
	case OUTPUT:
		ports[i]->PCR[n] &= ~PORT_PCR_PE(1); // Poner en 0 el Pull Enable
		gpio[i]->PDDR |= (1UL<<n);
		break;
	}
}

/**
 * @brief Write a HIGH or a LOW value to a digital pin
 * @param id the pin to write (according PORTNUM2PIN)
 * @param val Desired value (HIGH or LOW)
 */
void digitalWrite (uint8_t pin, uint8_t v)
{
	//assert(pin < 6*32);

	uint8_t i = pin/32; // Numero del puerto (A,B,C,D,E)
	uint8_t n = pin%32; // Numero del pin [0:31]
	static GPIO_Type * const gpio[] = GPIO_BASE_PTRS;

	gpio[i]->PDOR = ((gpio[i]->PDOR & (~(1<<n))) | (v<<n));

}

/**
 * @brief Toggle the value of a digital pin (HIGH<->LOW)
 * @param id the pin to toggle (according PORTNUM2PIN)
 */
void digitalToggle (uint8_t pin)
{
	//assert(pin >= 6*32);

	uint8_t i = pin/32; // Numero del puerto (A,B,C,D,E)
	uint8_t n = pin%32; // Numero del pin [0:31]
	static GPIO_Type * const gpio[] = GPIO_BASE_PTRS;
	gpio[i]->PTOR |= 1<<n;
}

/**
 * @brief Reads the value from a specified digital pin, either HIGH or LOW.
 * @param id the pin to read (according PORTNUM2PIN)
 * @return HIGH or LOW
 */
uint8_t digitalRead (uint8_t pin)
{
	assert(pin >= 6*32);

	uint8_t i = pin/32; // Numero del puerto (A,B,C,D,E)
	uint8_t n = pin%32; // Numero del pin [0:31]
	static GPIO_Type * const gpio[] = GPIO_BASE_PTRS;
	return gpio[i]->PDIR & ~(1<<n);
}
