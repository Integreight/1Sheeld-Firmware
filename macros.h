/*

  Project:       1Sheeld Firmware 
  File:          macros.h
  
  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
                 
  Date:          2014.5

*/

#ifndef MACROS_H_
#define MACROS_H_

#define CLR_BIT(var, bit) var &= (~(1<<bit))
#define SET_BIT(var, bit) var |= (1<<bit)
#define TOG_BIT(var, bit) var ^= (1<<bit)

#define ENABLED    1
#define DISABLED   0

// port arrays defs
#define PA 0
#define PB 1
#define PC 2
#define PD 3
#define PE 4


// timer defs
#define NOT_ON_TIMER 255
#define TIMER0  0
#define TIMER1A 1
#define TIMER1B 2
#define TIMER2  3
#define TIMER3A 4
#define TIMER3B 5

// values
#define HIGH 0x01
#define LOW  0x00

#define OUTPUT 0x01
#define INPUT  0x00

#define true 0x01
#define false 0x00

#endif /* INCFILE1_H_ */