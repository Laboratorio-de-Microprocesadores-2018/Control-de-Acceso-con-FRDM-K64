#include "SysTick.h"

typedef struct{
	SysTickFnc f;
	uint32_t resetValue;
	uint32_t count;
}SysTickCallback;


static SysTickCallback callbacks[SYSTICK_MAX_CALLBACKS];
static int callbacksSize;

uint32_t sysTickInit ()
{
	uint32_t ticks = SYSTICK_ISR_PERIOD_S*__CORE_CLOCK__;

	if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)
	    return (0UL);

	SysTick->CTRL = 0x00;  // Disable the timer
	SysTick->LOAD = ticks -1;
	SysTick->VAL  = 0UL;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk;
	return (1UL);
}

bool sysTickAddCallback(SysTickFnc fnc,float period)
{
	if(callbacksSize < SYSTICK_MAX_CALLBACKS)
	{
		callbacks[callbacksSize].f = fnc;
		callbacks[callbacksSize].resetValue = period/SYSTICK_ISR_PERIOD_S;
		callbacks[callbacksSize].count = period/SYSTICK_ISR_PERIOD_S;
		callbacksSize++;
		return true;
	}
	else return false;
}

bool sysTickAddDelayCall(SysTickFnc fnc,float time)
{
	if(callbacksSize < SYSTICK_MAX_CALLBACKS)
	{
		callbacks[callbacksSize].f = fnc;
		callbacks[callbacksSize].resetValue = 0;
		callbacks[callbacksSize].count = time/SYSTICK_ISR_PERIOD_S;
		callbacksSize++;
		return true;
	}
	else return false;
}


__ISR__ SysTick_Handler(void)
{
	// Iterate all registered functions
	for(int i=0;i<callbacksSize;i++)
	{
		callbacks[i].count--;

		if(callbacks[i].count==0)
		{
			// If timeouts, execute
			(*callbacks[i].f)();
			// And either reset the counter, or unregister
			if(callbacks[i].resetValue!=0)
				callbacks[i].count = callbacks[i].resetValue;
			else
			{
				for(int j=i+1;j<callbacksSize;j++)
				{
					callbacks[j-1]=callbacks[j];
				}
				callbacksSize--;
				i--;
			}
		}
	}
}
