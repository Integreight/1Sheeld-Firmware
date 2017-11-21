#ifndef TIMERS_H_
#define TIMERS_H_

#include "stdint.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "mapping.h"


/** 
 * the prescaler is set so that timer3 ticks every 64 clock cycles, and the
 * the overflow handler is called every 65,536 ticks.
 */
#define MICROSECONDS_PER_TIMER3_OVERFLOW (clockCyclesToMicroseconds(64 * 65536))
/* the whole number of milliseconds per timer3 overflow */
#define MILLIS_INC ((uint32_t)MICROSECONDS_PER_TIMER3_OVERFLOW / 1000)
/** the fractional number of milliseconds per timer3 overflow. we shift right
 *  by three to fit these numbers into a byte. (for the clock speeds we care
 *  about - 8 and 16 MHz - this doesn't lose precision.)
 */
#define FRACT_INC (((uint32_t)MICROSECONDS_PER_TIMER3_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

/**
 * @brief Enables timer overflow interrupt.
 * @param None.
 * @return None. 
 */
void enableTimer3Overflow();
/**
 * @brief Disables timer overflow interrupt.
 * @param None.
 * @return None. 
 */
void disableTimer3Overflow();
void enableTimer4Overflow();
void disableTimer4Overflow();

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
uint32_t millis();
/**
 * @brief Setup the millis timers  
 * @param None
 * @return  None  
 */
void setupMillisTimers();
#endif /* TIMERS_H_ */