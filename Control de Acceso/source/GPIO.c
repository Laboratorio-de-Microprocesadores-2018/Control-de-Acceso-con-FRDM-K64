#include "./gpio.h"
#include "hardware.h"
#include <string.h>

#define PIN_PORT(x) (x)/32UL
#define PIN_POS(x) (x)%32UL

static PORT_Type * const ports[] = PORT_BASE_PTRS;
static GPIO_Type * const gpio[] = GPIO_BASE_PTRS;

static pinIrqFun_t portAIRQs[32];
static pinIrqFun_t portBIRQs[32];
static pinIrqFun_t portCIRQs[32];
static pinIrqFun_t portDIRQs[32];
static pinIrqFun_t portEIRQs[32];
static pinIrqFun_t  * const portsIRQs[5]={portAIRQs,portBIRQs,portCIRQs,portDIRQs,portEIRQs};//


typedef struct  {
	uint8_t *arr;
	uint8_t size;
}buffer;

static uint8_t portAIndxs[32];
static uint8_t portBIndxs[32];
static uint8_t portCIndxs[32];
static uint8_t portDIndxs[32];
static uint8_t portEIndxs[32];
buffer buffA={portAIndxs,0};
buffer buffB={portBIndxs,0};
buffer buffC={portCIndxs,0};
buffer buffD={portDIndxs,0};
buffer buffE={portEIndxs,0};

static buffer *const portsIRQIndxs[5]={&buffA,&buffB,&buffC,&buffD,&buffE};



/**
 * @brief Configures the specified pin to behave either as an input or an output
 * @param pin the pin whose mode you wish to set (according PORTNUM2PIN)
 * @param mode INPUT, OUTPUT, INPUT_PULLUP or INPUT_PULLDOWN.
 */
void pinMode (uint8_t pin, uint8_t mode)
{
	if(pin >= 6*32)
		return;

	uint8_t portnum = PIN_PORT(pin); // Numero del puerto (A,B,C,D,E)
	uint8_t pinpos = PIN_POS(pin); // Numero del pin [0:31]


	// Configurar el pin como GPIO
	ports[portnum]->PCR[pinpos] |= PORT_PCR_MUX(1);

	switch(mode)
	{
	case INPUT:
		ports[portnum]->PCR[pinpos] &= ~PORT_PCR_PE(1); // Poner en 0 el Pull Enable
		gpio[portnum]->PDDR &= ~(1UL<<pinpos); // Poner en 0 el bit n del PDDR correspondiente
		break;
	case INPUT_PULLUP:
		ports[portnum]->PCR[pinpos] |= PORT_PCR_PE(1); // Poner en 1 el Pull Enable
		ports[portnum]->PCR[pinpos] |= PORT_PCR_PS(1); // Poner en 1 el Pull Select
		gpio[portnum]->PDDR &= ~(1UL<<pinpos);
		break;
	case INPUT_PULLDOWN:
		ports[portnum]->PCR[pinpos] |= PORT_PCR_PE(1); // Poner en 1 el Pull Enable
		ports[portnum]->PCR[pinpos] &= ~PORT_PCR_PS(1); // Poner en 0 el Pull Select
		gpio[portnum]->PDDR &= ~(1UL<<pinpos);
		break;
	case OUTPUT:
		ports[portnum]->PCR[pinpos] &= ~PORT_PCR_PE(1); // Poner en 0 el Pull Enable
		gpio[portnum]->PDDR |= (1UL<<pinpos);
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
	uint8_t value=v&1;
	//assert(pin < 6*32);

	uint8_t i = pin/32; // Numero del puerto (A,B,C,D,E)
	uint8_t n = pin%32; // Numero del pin [0:31]
	static GPIO_Type * const gpio[] = GPIO_BASE_PTRS;

	gpio[i]->PDOR = ((gpio[i]->PDOR & (~(1<<n))) | (value<<n));

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
	//assert(pin >= 6*32);

	uint8_t i = pin/32; // Numero del puerto (A,B,C,D,E)
	uint8_t n = pin%32; // Numero del pin [0:31]
	static GPIO_Type * const gpio[] = GPIO_BASE_PTRS;
	return ((gpio[i]->PDIR>>n) & 1);
}

/**
 * @brief Configures how the pin reacts when an IRQ event ocurrs
 * @param pin the pin whose IRQ mode you wish to set (according PORTNUM2PIN)
 * @param irqMode disable, risingEdge, fallingEdge or bothEdges
 * @param irqFun function to call on pin event
 * @return Registration succeed
 */
uint8_t pinConfigureIRQ (uint8_t pin, uint8_t irqMode, pinIrqFun_t irqFun)//Habria que desactivar interrupciones globalmente, no?
{

	uint8_t retVal=0;
	if(pin >= 6*32 || (irqMode!=IRQC_DISABLE && irqFun == NULL))
		retVal=1;
	else
	{
		portsIRQs[PIN_PORT(pin)][PIN_POS(pin)]=irqFun;

		//IRQC Mode set
		ports[PIN_PORT(pin)]->PCR[PIN_POS(pin)] &= ~PORT_PCR_IRQC_MASK;//Sets IRQC part in 0000
		switch(irqMode)
		{
			case IRQC_DISABLE:
				ports[PIN_PORT(pin)]->PCR[PIN_POS(pin)] |= PORT_PCR_IRQC(IRQC_DISABLE);//Sets IRQC for disable mode
				for(uint8_t i=0; i<portsIRQIndxs[PIN_PORT(pin)]->size ; i++)
				{
					if(portsIRQIndxs[PIN_PORT(pin)]->arr[i]==PIN_POS(pin))
					{
						for(uint8_t j=i; j<portsIRQIndxs[PIN_PORT(pin)]->size-1; j++)
						{
							portsIRQIndxs[PIN_PORT(pin)]->arr[j]=portsIRQIndxs[PIN_PORT(pin)]->arr[j+1];
						}
						portsIRQIndxs[PIN_PORT(pin)]->size--;
						break;
					}
				}

				break;
			case IRQC_INTERRUPT_RISING: case IRQC_INTERRUPT_FALLING	: case IRQC_INTERRUPT_EITHER:
				ports[PIN_PORT(pin)]->PCR[PIN_POS(pin)] |= PORT_PCR_IRQC(irqMode);//Sets IRQC for the specified mode
				uint8_t i=0;
				while(i<portsIRQIndxs[PIN_PORT(pin)]->size)
				{
					if(portsIRQIndxs[PIN_PORT(pin)]->arr[i]!=PIN_POS(pin))
						i++;
					else
						break;
				}
				if(i==portsIRQIndxs[PIN_PORT(pin)]->size)
				{
					portsIRQIndxs[PIN_PORT(pin)]->arr[i]=PIN_POS(pin);
					portsIRQIndxs[PIN_PORT(pin)]->size++;
				}

				break;
			default:
				retVal=1;
				break;
		}
	}
	return retVal;
}



//All IRQ Handlers work the same way
void PORTA_IRQHandler(void)
{
	if(buffA.size!=0)//if there is any pin enabled for interrupts
	{
		uint8_t pin=0;
		for(uint8_t i=0; i<buffA.size;++i)//for pin in PORTA
		{
			pin=buffA.arr[i];
			if( (ports[PA]->PCR[pin]&PORT_PCR_ISF_MASK) == PORT_PCR_ISF_MASK)//checks every pins' ISF
			{
				portAIRQs[pin]();//callback
				ports[PA]->PCR[pin] |= PORT_PCR_ISF(1);//Sets to 0 the interrupt status flag
			}
		}
	}

}
void PORTB_IRQHandler(void)
{
	if(buffB.size!=0)
	{
		uint8_t pin=0;
		for(uint8_t i=0; i<buffB.size;++i)
		{
			pin=buffB.arr[i];
			if( (ports[PB]->PCR[pin]&PORT_PCR_ISF_MASK) == PORT_PCR_ISF_MASK)
			{
				portBIRQs[pin]();
				ports[PB]->PCR[pin] |= PORT_PCR_ISF(1);//Sets to 0 the interrupt status flag
			}
		}
	}
}
void PORTC_IRQHandler(void)
{
	if(buffC.size!=0)
	{
		uint8_t pin=0;
		for(uint8_t i=0; i<buffC.size;++i)
		{
			pin=buffC.arr[i];
			if( (ports[PC]->PCR[pin]&PORT_PCR_ISF_MASK) == PORT_PCR_ISF_MASK)
			{
				portCIRQs[pin]();
				ports[PC]->PCR[pin] |= PORT_PCR_ISF(1);//Sets to 0 the interrupt status flag
			}
		}
	}
}
void PORTD_IRQHandler(void)
{
	if(buffD.size!=0)
	{
		uint8_t pin=0;
		for(uint8_t i=0; i<buffD.size;++i)
		{
			pin=buffD.arr[i];
			if( (ports[PD]->PCR[pin]&PORT_PCR_ISF_MASK) == PORT_PCR_ISF_MASK)
			{
				portDIRQs[pin]();
				ports[PD]->PCR[pin] |= PORT_PCR_ISF(1); //Sets to 0 the interrupt status flag by setting a 1
			}
		}
	}
}
void PORTE_IRQHandler(void)
{
	if(buffE.size!=0)
	{
		uint8_t pin=0;
		for(uint8_t i=0; i<buffE.size;++i)
		{
			pin=buffE.arr[i];
			if( (ports[PE]->PCR[pin]&PORT_PCR_ISF_MASK) == PORT_PCR_ISF_MASK)
			{
				portEIRQs[pin]();
				ports[PE]->PCR[pin] |= PORT_PCR_ISF(1);//Sets to 0 the interrupt status flag
			}
		}
	}
}
