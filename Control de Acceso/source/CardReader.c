#include "CardReader.h"
#include "gpio.h"


#define PIN_DATA PORTNUM2PIN(PD,2)
#define PIN_CLOCK PORTNUM2PIN(PC,4)
#define PIN_ENABLE PORTNUM2PIN(PD,0)



#define WORD_SIZE 5	// size in bits of each word
#define WORD_PARITY_BIT(x) (1 & ((x)>>WORD_SIZE) )

static void enableCallback(void);
static void newBit(void);
static void dataSignalChanged(void);
static void processWord(void);
static uint8_t getParity(void);
static void processRawData(void);

static uint8_t buffer[40];
static uint8_t data[19];
static _Bool dataReady;
static	uint8_t shiftCount;
static	uint8_t word;
static	uint8_t buffIndex;
static 	uint8_t dataValue;//Value of the last input bit
static 	uint8_t error;



void initCardReader(void)
{

	pinConfigureIRQ (PIN_ENABLE, IRQC_INTERRUPT_EITHER	, enableCallback );
	pinConfigureIRQ (PIN_CLOCK, IRQC_INTERRUPT_RISING	, newBit);
	pinConfigureIRQ (PIN_DATA, IRQC_INTERRUPT_FALLING	, dataSignalChanged );

}



static void enableCallback(void)
{
	if(digitalRead(PIN_ENABLE)==1)//in the raising edge case the raw data block obtained is processed
	{
		processRawData();
	}
	else//in the falling edge case the card is starting to be passed, so the driver resets
	{
		dataReady=0;
		shiftCount=0;
		buffIndex=0;
		word=0;
		dataValue=0;
	}
}


static void newBit(void)
{
	if(shiftCount== WORD_SIZE )
	{
		shiftCount=0;
		word=0;
	}
	word|= (dataValue & 1) << shiftCount;
	if(++shiftCount == WORD_SIZE)
		processWord();


}

static void dataSignalChanged(void)
{
	dataValue=1;//if there was a transition between rising edges the corresponding data bit is 1
}

static void processWord()
{

	if(getParity()==WORD_PARITY_BIT(word))
	{
		word&=0x0F;//clear MS nibble
		if(buffIndex<40)
		{
			buffer[buffIndex++]=word;
		}
		else
			error=1;

	}
	else
		error=1;

	dataReady = 1;
}

// Source: https://www.geeksforgeeks.org/program-to-find-parity/
/* Function to get parity of number n. It returns 1
   if n has odd parity, and returns 0 if n has even
   parity */
static uint8_t getParity()//se hace generica?
{
	uint8_t parity = 0;
    while (word)
    {
        parity = !parity;
        word      = word & (word - 1);
    }
    return parity;
}


static void processRawData(void)
{

}





