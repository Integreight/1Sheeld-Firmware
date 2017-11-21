#ifndef MACROS_H_
#define MACROS_H_

#define CLR_BIT(var, bit) var &= (~(1<<bit))
#define SET_BIT(var, bit) var |= (1<<bit)
#define TOG_BIT(var, bit) var ^= (1<<bit)

#define ENABLED    1
#define DISABLED   0
#define F_CPU 16000000UL

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

#define true 0x01
#define false 0x00


#endif /* INCFILE1_H_ */