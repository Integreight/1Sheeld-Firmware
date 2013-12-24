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
#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PE 5


// timer defs
#define NOT_ON_TIMER 0
#define TIMER0  1
#define TIMER1A 2
#define TIMER1B 3
#define TIMER2  4
#define TIMER3A 5
#define TIMER3B 6



#endif /* INCFILE1_H_ */