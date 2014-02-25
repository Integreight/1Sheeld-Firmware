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

unsigned  volatile int count2=0;
unsigned volatile int count=0;
void timer_Ovf_enable()
{
	SET_BIT(TIMSK,TOIE2);
	count2 =0 ;
}

void timer_Ovf_disable()
{
	CLR_BIT(TIMSK,TOIE2);
	CLR_BIT(PORTA,6);
}

ISR(TIMER2_OVF_vect)
{	
	if(count%10000==0)
	{
		TOG_BIT(PORTA,6);	
		count2++;
	} 
	
	if(count2>=5)
	{
		timer_Ovf_disable();
	}
	count++;
}
