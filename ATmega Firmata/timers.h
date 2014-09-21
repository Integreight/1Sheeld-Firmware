/*

  Project:       1Sheeld Firmware
  File:          timers.h

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
  
  Date:          2014.5

*/

/**
 * @file timers.h
 * @brief This Library illustrates 1Sheeld's timers 	
 * @author Integreight
 * @version 1.1
 */

#ifndef TIMERSa_H_
#define TIMERSa_H_
#include <avr/io.h>
#include <avr/interrupt.h>
#include "CommonMacros.h"
#include "atmega162_mapping.h"
// the prescaler is set so that timer0 ticks every 64 clock cycles, and the
// the overflow handler is called every 256 ticks.
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC ((unsigned long)MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)
// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC (((unsigned long)MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

/**
 * @brief Enable the Interrupt Overflow.
 * @param None.
 * @return None. 
 */
void enableTimerOverflow();
/**
 * @brief Disable the Interrupt Overflow.
 * @param None.
 * @return None. 
 */
void disableTimerOverflow();
/**
 * @brief turns on the Tx Leds on 1Sheeld board.
 * @param None.
 * @return None. 
 */
void enableRxLedBlinking();
/**
 * @brief turns on the Rx Leds on 1Sheeld board.
 * @param None.
 * @return None. 
 */
void enableTxLedBlinking();
/**
 * @brief Returns the number of milliseconds since the Arduino board began running the current program.
 * @param None.
 * @return Number of milliseconds since the program started (unsigned long). 
 */
unsigned long millis();
#endif /* TIMERS_H_ */