/*

  Project:       1Sheeld Firmware 
  File:          pwm.h

  Compiler:      Arduino avr-gcc 4.3.2

  Author:        Integreight
                 
  Date:          2014.5

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



void pwm_Setup(uint8);
void pwm_SetDutyCycle(uint8, uint8);





#endif /* PWM_H_ */