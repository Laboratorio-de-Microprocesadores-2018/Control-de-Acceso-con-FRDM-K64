#include "Buzzer.h"

#include "Board.h"
#include "GPIO.h"
#include "SysTick.h"

#define BUZZER_PIN PORTNUM2PIN(PA,9)

static void startBuzzer();
static void stopBuzzer();

void initBuzzer()
{
	pinMode(BUZZER_PIN,OUTPUT);
}

void bip()
{
	digitalWrite(BUZZER_PIN,1);
	sysTickAddDelayCall(&stopBuzzer,BIP_TIME_MS * MS2S);
}

void doubleBip()
{
	digitalWrite(BUZZER_PIN,1);
	sysTickAddDelayCall(&stopBuzzer,DOUBLE_BIP_TIME_MS * MS2S);
	sysTickAddDelayCall(&startBuzzer,2*DOUBLE_BIP_TIME_MS * MS2S);
	sysTickAddDelayCall(&stopBuzzer,3*DOUBLE_BIP_TIME_MS * MS2S);
}

void longBip()
{
	digitalWrite(BUZZER_PIN,1);
	sysTickAddDelayCall(&stopBuzzer,LONG_BIP_TIME_MS * MS2S);
}

static void stopBuzzer()
{
	digitalWrite(BUZZER_PIN,0);
}

static void startBuzzer()
{
	digitalWrite(BUZZER_PIN,1);
}



