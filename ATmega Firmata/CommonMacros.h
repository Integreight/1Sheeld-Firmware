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



#endif /* INCFILE1_H_ */