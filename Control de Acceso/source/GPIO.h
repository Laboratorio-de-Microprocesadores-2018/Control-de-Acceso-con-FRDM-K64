/////////////////////////////////////////////////////////////////////////////////
//                              CONTROL DE ACCESO                              //
//          Grupo 3 - Laboratorio de Microprocesadores - ITBA - 2018           //
//	                                                                           //
/////////////////////////////////////////////////////////////////////////////////

/**
 * @file     GPIO.h
 * @brief    GPIO driver to handle pin configurations and IRQ setups
 * @author   Diego juarez
 * @author   Tobías Lifschitz
 */

#ifndef _GPIO_H_
#define _GPIO_H_

/////////////////////////////////////////////////////////////////////////////////
//                             Included header files                           //
/////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <assert.h>
#include <Board.h>

/////////////////////////////////////////////////////////////////////////////////
//                    Enumerations, structures and typedefs                    //
/////////////////////////////////////////////////////////////////////////////////

typedef void (*pinIrqFun_t)(void);

enum {
    GPIO_IRQ_MODE_DISABLE,
    GPIO_IRQ_MODE_RISING_EDGE,
    GPIO_IRQ_MODE_FALLING_EDGE,
    GPIO_IRQ_MODE_BOTH_EDGES,

    GPIO_IRQ_CANT_MODES
};

/////////////////////////////////////////////////////////////////////////////////
//                         Global function prototypes                          //
/////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Configures the specified pin to behave either as an input or an output
 * @param pin the pin whose mode you wish to set (according PORTNUM2PIN)
 * @param mode INPUT, OUTPUT, INPUT_PULLUP or INPUT_PULLDOWN.
 */
void pinMode (uint8_t pin, uint8_t mode);

/**
 * @brief Configures how the pin reacts when an IRQ event ocurrs
 * @param pin the pin whose IRQ mode you wish to set (according PORTNUM2PIN)
 * @param irqMode disable, risingEdge, fallingEdge or bothEdges
 * @param irqFun function to call on pin event
 * @return Registration succeed
 */
uint8_t pinConfigureIRQ (uint8_t pin, uint8_t irqMode, pinIrqFun_t irqFun);

/**
 * @brief Write a HIGH or a LOW value to a digital pin
 * @param id the pin to write (according PORTNUM2PIN)
 * @param val Desired value (HIGH or LOW)
 */
void digitalWrite (uint8_t pin, uint8_t value);

/**
 * @brief Toggle the value of a digital pin (HIGH<->LOW)
 * @param id the pin to toggle (according PORTNUM2PIN)
 */
void digitalToggle (uint8_t pin);

/**
 * @brief Reads the value from a specified digital pin, either HIGH or LOW.
 * @param id the pin to read (according PORTNUM2PIN)
 * @return HIGH or LOW
 */
uint8_t digitalRead (uint8_t pin);

/////////////////////////////////////////////////////////////////////////////////
#endif // _GPIO_H_
