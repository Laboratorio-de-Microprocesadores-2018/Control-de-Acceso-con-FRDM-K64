#include "Buzzer.h"

#include "Board.h"
#include "GPIO.h"
#include "SysTick.h"

/////////////////////////////////////////////////////////////////////////////////
//                       Constants and macro definitions                       //
/////////////////////////////////////////////////////////////////////////////////
#define BUZZER_PIN PORTNUM2PIN(PD,3)

/////////////////////////////////////////////////////////////////////////////////
//                   Local function prototypes ('static')                      //
/////////////////////////////////////////////////////////////////////////////////
/**
 *
 */
static void startBuzzer();

/**
 *
 */
static void toggleBuzzer();

/**
 *
 */
static void stopBuzzer();

/////////////////////////////////////////////////////////////////////////////////
//                   Local variable definitions ('static')                     //
/////////////////////////////////////////////////////////////////////////////////

static int soundOn;
static int soundNow;
static Tone tone;

/////////////////////////////////////////////////////////////////////////////////
//                           Functions definitions 							   //
/////////////////////////////////////////////////////////////////////////////////

void initBuzzer()
{
	pinMode(BUZZER_PIN,OUTPUT);
	sysTickAddCallback(&toggleBuzzer,1/(float)BUZZER_FREQ);
}

void bip(Tone t)
{
	tone = t;
	startBuzzer();
	sysTickAddDelayCall(&stopBuzzer,BIP_TIME_MS * MS2S);
}

void doubleBip(Tone t)
{
	tone = t;
	startBuzzer();
	sysTickAddDelayCall(&stopBuzzer,3*DOUBLE_BIP_TIME_MS * MS2S);
	sysTickAddDelayCall(&startBuzzer,2*DOUBLE_BIP_TIME_MS * MS2S);
	sysTickAddDelayCall(&stopBuzzer,DOUBLE_BIP_TIME_MS * MS2S);
}

void longBip(Tone t)
{
	tone = t;
	startBuzzer();
	sysTickAddDelayCall(&stopBuzzer,LONG_BIP_TIME_MS * MS2S);
}




static void startBuzzer()
{
	soundOn=1;
}

static void toggleBuzzer()
{
	if(soundOn==1)
	{
		if(tone==LOW_PITCH)
		{
			if(soundNow)
			{
				digitalToggle(BUZZER_PIN);
				soundNow=0;
			}
			else
				soundNow=1;
		}
		else
			digitalToggle(BUZZER_PIN);
	}
}

static void stopBuzzer()
{
	soundOn=0;
	digitalWrite(BUZZER_PIN,0);
}


