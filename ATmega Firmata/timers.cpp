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
#define F_CPU 16000000UL
unsigned  volatile int countRx=0;
unsigned  volatile int countTx=0;
unsigned volatile int count=0;
volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;
void timer_Ovf_enable()
{
	SET_BIT(TIMSK,TOIE2);
}

void enableRxLed(){
	//isRxEnabled=1;
	countRx =1 ;
}

void enableTxLed(){
	//isTxEnabled=1;
	countTx =1 ;
}
unsigned long millis()
{
	unsigned long m;
	uint8_t oldSREG = SREG;

	// disable interrupts while we read timer0_millis or we might get an
	// inconsistent value (e.g. in the middle of a write to timer0_millis)
	cli();
	m = timer0_millis;
	SREG = oldSREG;

	return m;
}
ISR (TIMER0_OVF_vect)
{
	unsigned long m = timer0_millis;
	unsigned char f = timer0_fract;
	m += MILLIS_INC;
	f += FRACT_INC;
	if (f >= FRACT_MAX) {
	f -= FRACT_MAX;
	m += 1;
	  }

	timer0_fract = f;
	timer0_millis = m;
	timer0_overflow_count++;
}
ISR(TIMER2_OVF_vect)
{	
	if(count%200==0)
	{
		if(countRx>0&&countRx<=6){
			TOG_BIT(PORTA,6);	
			countRx++;
		}
		if(countTx>0&&countTx<=6){
			TOG_BIT(PORTA,7);
			countTx++;
		}
		
	if(countRx>=6&&countTx>=6)
	{
		CLR_BIT(TIMSK,TOIE2);
		SET_BIT(PORTA,6);
		SET_BIT(PORTA,7);
	}
	else if(countRx>=6){
		//isRxEnabled=false;
		countRx=0;
		SET_BIT(PORTA,6);
	}
	else if(countTx>=6){
		//isTxEnabled=false;
		countTx=0;
		SET_BIT(PORTA,7);
	}
	} 
	

	count++;
}
