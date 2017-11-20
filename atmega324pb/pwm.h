#ifndef PWM_H_
#define PWM_H_

#include "stdint.h"
#include <avr/io.h>
#include "macros.h"
/**
 * @brief Setup the prescalers of the timers except timer3=16 and timer 2=32.
 * @param timer The number of timer 0 --> 5.
 * @return None. 
 */
void initPwm(uint8_t);
/**
 * @brief Set the duty cycle of the signal on timers.
 * @param dutyCycle value of the signals duty cycle.
 * @param timerNo The number of timer 0 --> 5.
 * @return None. 
 */
void setPwmDutyCycle(uint8_t, uint8_t);
/**
 * @brief turning of the certain timers used by the library 
 * @param timer number of the timer to be turned off  
 * @return  None  
 */
void turnOffPWM(uint8_t);

uint8_t digitalPinToTimer(uint8_t pin);
#endif /* PWM_H_ */