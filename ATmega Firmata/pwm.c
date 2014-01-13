/*
 * pwm.c
 *
 * Created: 01/01/2014 09:47:40 م
 *  Author: HP
 */ 
#include <avr/io.h>
#include "sys.h"
#include "pwm.h"

void pwm_Setup(uint8 timer)
{
	switch(timer)
	{
		case TIMER_00 :
			TCCR0|=(1<<WGM00)|(1<<WGM01)|(1<<CS00)|(1<<COM01);
			break;
		
		case TIMER_1A:
			TCCR1A|=(1<<WGM10)|(1<<COM1A1);
			TCCR1B|=(1<<WGM12)|(1<<CS10);	
			break;
		
		case TIMER_1B:
			TCCR1A|=(1<<WGM10)|(1<<COM1B1);
			TCCR1B|=(1<<WGM12)|(1<<CS10);	
			break;
		
		case TIMER_02:
			TCCR2|=(1<<WGM20)|(1<<WGM21)|(1<<CS20)|(1<<COM01);
			break;
		
		case TIMER_3A:
			TCCR3A|=(1<<WGM30)|(1<<COM3A1);
			TCCR3B|=(1<<WGM32)|(1<<CS30);
			break;
		
		case TIMER_3B:
			TCCR3A|=(1<<WGM30)|(1<<COM3B1);
			TCCR3B|=(1<<WGM32)|(1<<CS30);
			break;	
		
		default:
			break;	
				 	
	}
}

void pwm_SetDutyCycle(uint8 dutyCycle, uint8 timer_No )
{
	
	switch(timer_No)
	{
		case TIMER_00 :
			OCR0 = dutyCycle;
			break;
		
		case TIMER_1A:
			OCR1A = dutyCycle;
			break;
		
		case TIMER_1B:
			OCR1B = dutyCycle;
			break;
		
		case TIMER_02:
			OCR2 = dutyCycle;
			break;
		
		case TIMER_3A:
			OCR3A = dutyCycle;
			break;
			
		case TIMER_3B:
			OCR3B = dutyCycle;	
			break;
		
		default:
			break;
		
	}
}


