/*

  Project:       1Sheeld Firmware
  File:          pwm.h

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
  
  Date:          2014.5

*/

/**
 * @file pwm.h
 * @brief This Library deals with the Timers to generate pulse width modulation signals 	
 * @author Integreight
 * @version 1.1
 */



#ifndef PWM_H_
#define PWM_H_
#include "stdint.h"
#include <avr/io.h>
#include "mapping.h"

#define TIMER_00		0
#define TIMER_1A		1
#define TIMER_1B		2
#define TIMER_02		3
#define TIMER_3A		4
#define TIMER_3B		5


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





#endif /* PWM_H_ */