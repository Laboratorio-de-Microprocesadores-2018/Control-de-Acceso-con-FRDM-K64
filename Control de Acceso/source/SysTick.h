/***************************************************************************//**
  @file     SysTick.h
  @brief    SysTick driver
  @author   Grupo 3 - Laboratorio de Microprocesadores - 2018
 ******************************************************************************/

#ifndef _SYSTICK_H_
#define _SYSTICK_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>
#include "hardware.h"
#include <stdbool.h>
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define SYSTICK_MAX_CALLBACKS 10
#define MS2S (1/1000.0)
#define SYSTICK_ISR_PERIOD_S (10*MS2S)  // 10 ms period
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef void(*SysTickFnc)(void);

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Init systick driver
 * @return Init succeed
 */
uint32_t sysTickInit ();
/**
 * @brief Register a periodic call to a function.
 * @param period Period of the function call.
 * @return Register succeed. If false, max. number of callbacks reached.
 */
bool sysTickAddCallback(SysTickFnc fnc,float period);

/**
 * @brief Register a single call to a function, after a certain delay.
 * @param time Time to wait before the function call.
 * @return Register succeed. If false, max. number of callbacks reached.
 */
bool sysTickAddDelayCall(SysTickFnc fnc,float time);
//*******************************************************************************

#endif // _SYSTICK_H_
