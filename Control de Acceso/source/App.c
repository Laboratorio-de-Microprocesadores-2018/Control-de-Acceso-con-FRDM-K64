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
#include "Display.h"
#include "CardReader.h"

void App_Init (void)
{
	initMultiplexer();
	initKeyboard();
	sysTickInit();
	pinMode(PIN_LED_BLUE,OUTPUT);
	digitalWrite(PIN_LED_BLUE,HIGH);
	//initDisplay();
	//initCardReader();
	//initBuzzer();
}

void App_Run (void)
{
	static char arr[100];
	int i=0;
	while(1)
	{
		KeyboardEvent ev = getKeyboardEvent();
		if(ev.type==KB_KEY_DOWN)
		{
			if(ev.charCode=='*')
				break;
			else
			{
				arr[i] = ev.charCode;
				i++;
			}
			/*
			if(isnum(c))
				putchar(c)
			else if (c=='*')
				erase();
			else if (c=='#')
				clearDisplay();
			*/
		}
	}
	while(1);
}
