#include <Board.h>
#include "SysTick.h"
// Structure to store SysTick callbacks
typedef struct{
	SysTickFnc f;        // Function pointer
	uint32_t resetValue; // Reset value for the counter
	uint32_t count;      // Counter
}SysTickCallback;

// Static array to store callbacks
static SysTickCallback callbacks[SYSTICK_MAX_CALLBACKS];
static int callbacksSize;

uint32_t sysTickInit()
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
	// If the array is not full
	if(callbacksSize < SYSTICK_MAX_CALLBACKS)
	{
		callbacks[callbacksSize].f = fnc;
		callbacks[callbacksSize].resetValue = (uint32_t)(period/SYSTICK_ISR_PERIOD_S+0.5);
		callbacks[callbacksSize].count = callbacks[callbacksSize].resetValue;
		callbacksSize++;
		return true;
	}
	else return false;
}

bool sysTickAddDelayCall(SysTickFnc fnc,float time)
{
	// If the array is not full
	if(callbacksSize < SYSTICK_MAX_CALLBACKS)
	{
		callbacks[callbacksSize].f = fnc;
		callbacks[callbacksSize].resetValue = 0; // The reset counter is cero, call only once!
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
