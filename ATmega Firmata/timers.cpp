/*
 * timers.cpp
 *
 * Created: 24/02/2014 07:35:34 م
 *  Author: HP
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "CommonMacros.h"
#include "timers.h"

unsigned int count2=0;
void timer_Ovf_enable()
{
	SET_BIT(TIMSK,TOIE2);
}

void timer_Ovf_disable()
{
	CLR_BIT(TIMSK,TOIE2);
	
}

ISR(TIMER2_OVF_vect)
{
	unsigned int count=0;
	
	if(count%10==0)
	{
		TOG_BIT(PORTA,6);	
	} 
	
	if(count2%1000==0)
	{
		CLR_BIT(TIMSK,TOIE2);
		CLR_BIT(PORTA,6);
		count2=0;
	}
	count++;
	count2++;
}
