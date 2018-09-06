/////////////////////////////////////////////////////////////////////////////////
//                             Included header files                           //
/////////////////////////////////////////////////////////////////////////////////
#include "CardReader.h"
#include "gpio.h"

/////////////////////////////////////////////////////////////////////////////////
//                       Constants and macro definitions                       //
/////////////////////////////////////////////////////////////////////////////////
#define SS_char 0x0B
#define FS_char 0x0D
#define ES_char 0x0F

#define DATA_BUFFER_LENGTH 40
#define BIT_BUFFER_LENGTH 250
#define DATA_MIN_COUNT 7
#define WORD_SIZE 5	// size in bits of each word
#define WORD_PARITY_BIT(x) (1 & ((x)>>(WORD_SIZE-1)) )

/////////////////////////////////////////////////////////////////////////////////
//                   Local function prototypes ('static')                      //
/////////////////////////////////////////////////////////////////////////////////
static void enableCallback(void);
static void newBit(void);
static void dataSignalChanged(void);
static void processWord(uint8_t w);
static uint8_t getParity(uint8_t w);
static void processRawData(void);
static void checkLRCparity();
void setError(_Bool s);

/////////////////////////////////////////////////////////////////////////////////
//                   Local variable definitions ('static')                     //
/////////////////////////////////////////////////////////////////////////////////
static uint8_t buffer[DATA_BUFFER_LENGTH];

static	uint8_t bitCount;
static	uint8_t clockCounter;
static	uint8_t bitCount;
static uint8_t bitBuffer[BIT_BUFFER_LENGTH];


static _Bool dataReady;
static _Bool savingData;
static _Bool searchingSS;
static _Bool wait4LRC;
static	int8_t shiftCount;
static	uint8_t word;
static	uint8_t buffIndex;
static 	uint8_t dataValue;//Value of the last input bit
static 	uint8_t error;
static uint8_t pin_enable;
static uint8_t pin_clock;
static uint8_t pin_data;

void initCardReader(uint8_t enable_pin, uint8_t clock_pin, uint8_t data_pin)
{
	pin_enable = enable_pin;
	pin_clock = clock_pin;
	pin_data = data_pin;
	pinMode(enable_pin, INPUT);
	pinMode(clock_pin, INPUT);
	pinMode(data_pin, INPUT);

	pinConfigureIRQ (enable_pin, IRQC_INTERRUPT_EITHER	, enableCallback );
	pinConfigureIRQ (clock_pin, IRQC_INTERRUPT_RISING	, newBit);
	pinConfigureIRQ (data_pin, IRQC_INTERRUPT_FALLING	, dataSignalChanged );
}

_Bool isDataReady(void)
{
	return dataReady;
}

void setError(_Bool s)		// for debugging
{
	if(s == true)
		error = true;
	else
		error = false;
}

_Bool cardInserted(void)
{
	_Bool retVal = false;
	if(digitalRead(pin_enable)==LOW)
		retVal = true;

	return retVal;
}
static void enableCallback(void)
{
	if(digitalRead(pin_enable)==HIGH)//in the raising edge case the raw data block obtained is processed
	{
		processRawData();
	}
	else//in the falling edge case the card is starting to be passed, so the driver resets
	{
		dataReady=false;
		shiftCount=0;		// to fix the offset of the first clock of the card
		buffIndex=0;
		word=0;
		dataValue=0;
		savingData = false;
		searchingSS = true;
		wait4LRC = false;
		setError(false);
		for(uint8_t i=0 ; i<DATA_BUFFER_LENGTH ; i++)
			buffer[i]=0;

		bitCount = 0;
		clockCounter = 0;
		for(uint8_t i=0 ; i<BIT_BUFFER_LENGTH ; i++)
			bitBuffer[i]=0;
	}
}


static void newBit(void)
{
	clockCounter++;
	bitBuffer[bitCount++] = dataValue & 1;
	if(searchingSS == true)
	{
		word = word>>1;
		word |= (dataValue & 1) << 4;
		processWord(word);
		if(searchingSS == false)
			word = 0;
	}
	else
	{
		if(shiftCount== WORD_SIZE )
		{
			shiftCount=0;
			word=0;
		}
		word |= (dataValue & 1) << shiftCount;

		if(++shiftCount == WORD_SIZE)
			processWord(word);
	}
	dataValue = 0;
}

static void dataSignalChanged(void)
{
	dataValue=1;//if there was a transition between rising edges the corresponding data bit is 1
}

static void processWord(uint8_t w)
{
	if( (w & 0x0F) == SS_char )
	{
		savingData = true;
		searchingSS = false;
	}

	if(savingData == true || wait4LRC == true)
	{
		if(getParity(w))
		{
			if(wait4LRC == true)
				wait4LRC = false;

			w &= 0x0F;	//clear MS nibble
			if(buffIndex < 40)
				buffer[buffIndex++]=w;
			else
				setError(true);

		}
		else
			setError(true);
	}

	if( (w & 0x0F) == ES_char )	// if the End Sentinel was saved, stop saving data and wait for LRC
	{
		savingData = false;
		wait4LRC = true;	// next word is LRC
	}
}

// Source: https://www.geeksforgeeks.org/program-to-find-parity/
/* Function to get parity of number n. It returns 1
   if n has odd parity, and returns 0 if n has even
   parity */

static uint8_t getParity(uint8_t w) //se hace generica?
{
	uint8_t parity = 1;
	uint8_t word2check = w;
	for(uint8_t i=1 ; i<5 ; i++)
		word2check ^= w>>i;

	parity &= word2check;

	return parity;
}


static void processRawData(void)
{
	checkLRCparity();
	dataReady = true;
	if(buffIndex < DATA_MIN_COUNT)
		setError(true);
	if(savingData == true)
		setError(true);
}

static void checkLRCparity()
{
	uint8_t parityAcc = 0;
	for(uint8_t i = 0 ; i < DATA_BUFFER_LENGTH - 1 ; i++)
	{
		parityAcc ^= buffer[i];
	}
	if(parityAcc != buffer[DATA_BUFFER_LENGTH-1])
		setError(true);

}

uint8_t getCardNumber(uint8_t * array)
{
	uint8_t i = 0;
	if(array != NULL && dataReady == true && error == false)
	{
		while( buffer[i+1] != FS_char && i < DATA_CARD_NUMBER_LENGTH)
		{
			++i;
			array[i-1] = buffer[i];
		}
	}

	dataReady = false;		// clear the dataReady flag
	return i;
}

