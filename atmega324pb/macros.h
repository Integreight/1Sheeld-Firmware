#ifndef MACROS_H_
#define MACROS_H_

#include <stdbool.h>
#include "avr/iom324pb.h"

#define F_CPU 16000000UL

#define NOT_A_PIN 255
#define NOT_A_PORT 255

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
#define NOT_ON_TIMER    255
#define TIMER_0A		0
#define TIMER_0B		1
#define TIMER_1A		2
#define TIMER_1B		3
#define TIMER_2A		4
#define TIMER_2B		5
#define TIMER_4A		6
// values
#define HIGH 0x01
#define LOW  0x00

#define OUTPUT 0x01
#define INPUT  0x00



#define S_PA0 0x00
#define S_PA1 0x01
#define S_PA2 0x02
#define S_PA3 0x03
#define S_PA4 0x04
#define S_PA5 0x05
#define S_PA6 0x06
#define S_PA7 0x07

#define S_PB0 0x10
#define S_PB1 0x11
#define S_PB2 0x12
#define S_PB3 0x13
#define S_PB4 0x14
#define S_PB5 0x15
#define S_PB6 0x16
#define S_PB7 0x17

#define S_PC0 0x20
#define S_PC1 0x21
#define S_PC2 0x22
#define S_PC3 0x23
#define S_PC4 0x24
#define S_PC5 0x25
#define S_PC6 0x26
#define S_PC7 0x27

#define S_PD0 0x30
#define S_PD1 0x31
#define S_PD2 0x32
#define S_PD3 0x33
#define S_PD4 0x34
#define S_PD5 0x35
#define S_PD6 0x36
#define S_PD7 0x37

#define S_PE0 0x40 //P E has 7 pins only
#define S_PE1 0x41
#define S_PE2 0x42
#define S_PE3 0x43
#define S_PE4 0x44
#define S_PE5 0x45
#define S_PE6 0x46

#define isPinPwm(p)           ( ((p) == S_PB3) || ((p) == S_PD4) || ((p) == S_PD5) ||  ((p) == S_PD6) ||  ((p) == S_PD7)|| ((p) == S_PC4) )
#define isPinUART(p)		  ( ((p) == S_PC6) || ((p) == S_PC7) || ((p) == S_PD2)|| ((p) == S_PD3) )
#define clockCyclesPerMicrosecond() ( F_CPU /(double) 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )
#endif /* INCFILE1_H_ */