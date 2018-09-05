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


#include <ctype.h>
void App_Init (void)
{
	initMultiplexer();
	initKeyboard();
	sysTickInit();
	pinMode(PIN_LED_BLUE,OUTPUT);
	digitalWrite(PIN_LED_BLUE,HIGH);
	initDisplay();
	//initCardReader();
	//initBuzzer();
}

void App_Run (void)
{

	while(1)
	{
		KeyboardEvent ev = getKeyboardEvent();
		if(ev.type==KB_KEY_DOWN)
		{
			char c = ev.charCode;
			if(c=='*')
				erase();
			else if(c=='#')
				dispClear();
			else
				putchar(c);

		}
	}
	while(1);
}
