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

unsigned  volatile int countRx=0;
unsigned  volatile int countTx=0;
unsigned volatile int count=0;
//unsigned char isRxEnabled=0;
//unsigned char isTxEnabled=0;
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

ISR(TIMER2_OVF_vect)
{	
	if(count%100==0)
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
