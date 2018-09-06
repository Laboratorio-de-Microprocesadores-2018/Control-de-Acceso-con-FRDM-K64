/////////////////////////////////////////////////////////////////////////////////
//                              CONTROL DE ACCESO                              //
//          Grupo 3 - Laboratorio de Microprocesadores - ITBA - 2018           //
//	                                                                           //
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//                             Included header files                           //
/////////////////////////////////////////////////////////////////////////////////

#include "GPIO.h"
#include "SysTick.h"
#include "Multiplexer.h"
#include "Keyboard.h"
//#include "Display.h"
#include "CardReader.h"

#define PIN_DATA PORTNUM2PIN(PC,4)
#define PIN_CLOCK PORTNUM2PIN(PD,0)
#define PIN_ENABLE PORTNUM2PIN(PD,2)

void App_Init (void)
{
//	initMultiplexer();
//	initKeyboard();
//	sysTickInit();
	pinMode(PIN_LED_BLUE,OUTPUT);
	pinMode(PIN_LED_GREEN,OUTPUT);
	pinMode(PIN_LED_RED,OUTPUT);
	digitalWrite(PIN_LED_BLUE,HIGH);
	digitalWrite(PIN_LED_RED,HIGH);
	digitalWrite(PIN_LED_GREEN,HIGH);
	//initDisplay();
	initCardReader(PIN_ENABLE, PIN_CLOCK, PIN_DATA);
	NVIC_EnableIRQ(PORTA_IRQn);
	NVIC_EnableIRQ(PORTB_IRQn);
	NVIC_EnableIRQ(PORTC_IRQn);
	NVIC_EnableIRQ(PORTD_IRQn);
	NVIC_EnableIRQ(PORTE_IRQn);
	//initBuzzer();
}

void App_Run (void)
{
	uint8_t dataCount = 0;
	uint8_t cardData[DATA_CARD_NUMBER_LENGTH];
	for(uint8_t j=0 ; j < DATA_CARD_NUMBER_LENGTH ; j++)
		cardData[j] = 0;
	while(1)
	{
		if(isDataReady() == true)
		{
			dataCount = getCardNumber(cardData);
			if(dataCount > 0)
			{
				digitalWrite(PIN_LED_GREEN, LOW);
				digitalWrite(PIN_LED_RED, HIGH);
			}
			else
			{
				digitalWrite(PIN_LED_GREEN, HIGH);
				digitalWrite(PIN_LED_RED, LOW);
			}

		}
	}

}
