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
#include "Buzzer.h"
#include "Assert.h"

/** FW Profile function definitions */
#include "FwSmConstants.h"
#include "FwSmDCreate.h"
#include "FwSmConfig.h"
#include "FwSmAux.h"
#include "StateMachine.h"
#include "StateMachineData.h"

/** Structure to share data of events to state machine*/
static StateMachineData data;

/** State machine */
static FwSmDesc_t FSM;
typedef int SmEvent;
static void processEvent(SmEvent ev);


void App_Init (void)
{
	/** Create the state machine.*/
	FSM = StateMachineCreate(&data);

	/** Check that the it is properly configured */
	FwSmErrCode_t status = FwSmCheckRec(FSM);
	ASSERT(status != smSuccess);

	/** Start the SM*/
	FwSmStart(FSM);

	/** Drivers initialization */
	initKeyboard();
	initDisplay();
	initMultiplexer();
	sysTickInit();
	initCardReader();
	initBuzzer();
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
				if(data.bufferLen<BUFFER_SIZE)
				{
					data.buffer[data.bufferLen] = c;
					data.bufferLen++;
					processEvent(NUMBER);
				}
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
			if(c=='*')
				processEvent(RESET);
			if(c=='0')
				processEvent(SET_BRIGHTNESS);
		}
		else if(ev.type == KB_KEY_MULTI_PRESS)
		{

		}
	}
	if(isDataReady()==_crDataOk)
	{
		data.cardNumber = getCardNumber();
		data.cardPassed = true;
		processEvent(CARD);
	}
}

void processEvent(SmEvent ev)
{
	FwSmMakeTrans(FSM, ev);
}
