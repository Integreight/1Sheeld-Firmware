/*

  Project:       1Sheeld Firmware
  File:          pwm.h

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
  
  Date:          2014.5

*/

/**
 * @file pwm.h
 * @brief This Library deals with the Timers to generate Pulses 	
 * @author Integreight
 * @version 1.1
 */

#include <avr/io.h>
#include "sys.h"

#ifndef PWM_H_
#define PWM_H_
#define TIMER_00		0
#define TIMER_1A		1
#define TIMER_1B		2
#define TIMER_02		3
#define TIMER_3A		4
#define TIMER_3B		5


/**
 * @brief Setup the prescalers of the Timers except timer3=16 and timer 2=32.
 * @param timer number of the  timer selected.
 * @return None. 
 */
void pwm_Setup(uint8);
/**
 * @brief Set the duty cycle of the signal.
 * @param dutyCycle value of the signals duty.
 * @param timerNo number of timer selected.
 * @return None. 
 */
void pwm_SetDutyCycle(uint8, uint8);





#endif /* PWM_H_ */