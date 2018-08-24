
#ifndef _BOARD_H_
#define _BOARD_H_


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

enum { PA, PB, PC, PD, PE };
 
#define PORTNUM2PIN(po, pi)  (((po)<<5) + (pi)) // Ex: PTB22 -> PORTNUM2PIN(PB,22)

#ifndef INPUT
#define INPUT           0
#define OUTPUT          1
#define INPUT_PULLUP    2
#define INPUT_PULLDOWN  3
#endif // INPUT

#ifndef LOW
#define LOW     0
#define HIGH    1
#endif // LOW



/***** BOARD defines **********************************************************/


// On Board User LEDs
#define PIN_LED_RED     // ???
#define PIN_LED_GREEN   // ???
#define PIN_LED_BLUE    PORTNUM2PIN(PB,21) // PTB21

#define LED_ACTIVE      LOW
#define VAL2LED(v)      (!(v) == !LED_ACTIVE)


// On Board User Switches
#define PIN_SW2         // ???
#define PIN_SW3         // ???

#define SW_ACTIVE       // ???
#define SW_INPUT_TYPE   // ???


/*******************************************************************************
 ******************************************************************************/

#endif // _BOARD_H_
