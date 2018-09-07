
#include "hardware.h"

extern void App_Init (void);
extern void App_Run (void);


int main (void)
{
	hw_Init();
	hw_DisableInterrupts();
	App_Init(); /* Program-specific setup */
	hw_EnableInterrupts();
	
	__FOREVER__
		App_Run(); /* Program-specific loop  */
}
