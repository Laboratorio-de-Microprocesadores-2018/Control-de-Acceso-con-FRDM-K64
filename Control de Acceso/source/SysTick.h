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
#define MS2S (1/1000.0)
// Execution period of systicks callbacks
#define SYSTICK_ISR_PERIOD_S (10*MS2S)  // 10 ms period
// Maximum number of callbacks
#define SYSTICK_MAX_CALLBACKS 10

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef void(*SysTickFnc)(void);

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// Init SysTick driver
uint32_t sysTickInit ();
// Register a periodic call to a function
bool sysTickAddCallback(SysTickFnc fnc,float period);
// Register a single call to a function, after a certain delay
bool sysTickAddDelayCall(SysTickFnc fnc,float time);
//*******************************************************************************

#endif // _SYSTICK_H_
