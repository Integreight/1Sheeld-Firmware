/**
 * @file timers.h
 * @brief Timers driver for configuring and dealing with ATmega162 timers  	
 * @author Integreight
 * @version 1.1
 */

#ifndef TIMERS_H_
#define TIMERS_H_
#include "stdint.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "mapping.h"


/** 
 * the prescaler is set so that timer0 ticks every 64 clock cycles, and the
 * the overflow handler is called every 256 ticks.
 */
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))
/* the whole number of milliseconds per timer0 overflow */
#define MILLIS_INC ((uint32_t)MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)
/** the fractional number of milliseconds per timer0 overflow. we shift right
 *  by three to fit these numbers into a byte. (for the clock speeds we care
 *  about - 8 and 16 MHz - this doesn't lose precision.)
 */
#define FRACT_INC (((uint32_t)MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

/**
 * @brief Enables timer overflow interrupt.
 * @param None.
 * @return None. 
 */
void enableTimerOverflow();
/**
 * @brief Disables timer overflow interrupt.
 * @param None.
 * @return None. 
 */
void disableTimerOverflow();
/**
 * @brief Turns on Rx Leds blinking on 1Sheeld board.
 * @param None.
 * @return None. 
 */
void enableRxLedBlinking();
/**
 * @brief Turns on Tx Leds blinking on 1Sheeld board.
 * @param None.
 * @return None. 
 */
void enableTxLedBlinking();
/**
 * @brief Returns the number of milliseconds since 1Sheeld board began running.
 * @param None.
 * @return Number of milliseconds since the program started (unsigned long). 
 */
uint32_t millis();
/**
 * @brief Setup the millis timers  
 * @param None
 * @return  None  
 */
void setupMillisTimers();
#endif /* TIMERS_H_ */