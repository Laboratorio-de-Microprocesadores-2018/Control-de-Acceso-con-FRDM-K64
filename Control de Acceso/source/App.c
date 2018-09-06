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

/** StateMachine function definitions */
#include "StateMachine.h"

/** FW Profile function definitions */
#include "FwSmConstants.h"
#include "FwSmDCreate.h"
#include "FwSmConfig.h"
//#include "FwSmCore.h"
#include "FwSmAux.h"
#include "StateMachineData.h"

#include <ctype.h>


static StateMachineData data;
static FwSmDesc_t smDesc;
typedef int SmEvent;
static void processEvent(SmEvent ev);


void App_Init (void)
{
	/** Define the state machine descriptor (SMD) */
	smDesc = StateMachineCreate(&data);

	/** Check that the SM is properly configured */
	FwSmErrCode_t status = FwSmCheckRec(smDesc);
	if (status != smSuccess)
		while(1);

	/** Start the SM*/
	FwSmStart(smDesc);

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

	KeyboardEvent ev = getKeyboardEvent();
	if(ev.type!=KB_NO_EVT)
	{
		char c = ev.charCode;
		if(ev.type==KB_KEY_SINGLE_PRESS)
		{
			if(('0'<=c)&&(c<='9'))
			{
				data.buffer[data.bufferLen] = c;
				data.bufferLen++;
				processEvent(NUMBER);
			}
			else if(c=='*')
				processEvent(KEY_1);
			else if(c=='#')
				processEvent(KEY_2);
		}
		else if(ev.type == KB_KEY_LONG_PRESS)
		{
			if(c=='#')
				processEvent(LOGIN);
		}
		else if(ev.type == KB_KEY_MULTI_PRESS)
		{
			if(c=='0' && ev.clicks==2)
				processEvent(SET_BRIGHTNESS);
		}
	}
	if(1/*hayTarjeta*/)
	{
		//data.cardNumber=getnumber();
		data.cardPassed=true;
	}
}


void processEvent(SmEvent ev)
{
	FwSmMakeTrans(smDesc, ev);
}
