#include "SysTick.h"


uint32_t SysTick_Init (float time)
{
	uint32_t ticks = time*__CORE_CLOCK__;

	if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)
	    return (0UL);

	SysTick->CTRL = 0x00;  // Disable the timer
	SysTick->LOAD = ticks -1;
	SysTick->VAL  = 0UL;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk;
	return (1UL);
}
