/*
 * IncFile1.h
 *
 * Created: 16/02/2013 11:08:06 م
 *  Author: HP
 * Write any macro that is not related to specific file
 *
 */ 



#ifndef COMMON_MACROS_H_
#define COMMON_MACROS_H_

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
#define NOT_ON_TIMER 0
#define TIMER0  1
#define TIMER1A 2
#define TIMER1B 3
#define TIMER2  4
#define TIMER3A 5
#define TIMER3B 6

// values
#define HIGH 0x1
#define LOW  0x0

#define OUTPUT 0x1
#define INPUT  0x0



#endif /* INCFILE1_H_ */