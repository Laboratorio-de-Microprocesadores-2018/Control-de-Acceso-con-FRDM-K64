#include "SysTick.h"


uint32_t SysTick_Init (int period)
{
	uint32_t ticks = period/1000.0*__CORE_CLOCK__;

	if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)
	    return (0UL);

	SysTick->CTRL = 0x00;  // Disable the timer
	SysTick->LOAD = ticks -1;
	SysTick->VAL  = 0UL;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk;
	return (1UL);
}
