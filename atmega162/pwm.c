/*

  Project:       1Sheeld Firmware 
  File:          pwm.cpp

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
                 
  Date:          2014.5

*/
#include "config.h"
#include "pwm.h"

void initPwm(uint8_t timer)
{
	switch(timer)
	{
		// make 64 prescalar for all timers except timer3=16 and timer 2=32 
		case TIMER0 :
		TCCR0|=(1<<WGM00)|(1<<WGM01)|(1<<CS00)|(1<<COM01)|(1<<CS01);
		break;
		
		case TIMER1A:
		TCCR1A|=(1<<WGM10)|(1<<COM1A1);
		TCCR1B|=(1<<WGM12)|(1<<CS10)|(1<<CS11);
		break;
		
		case TIMER1B:
		TCCR1A|=(1<<WGM10)|(1<<COM1B1);
		TCCR1B|=(1<<WGM12)|(1<<CS10)|(1<<CS11);
		break;
		
		case TIMER2:
		TCCR2|=(1<<WGM20)|(1<<WGM21)|(1<<CS21)|(1<<CS20)|(1<<COM01);
		break;
		
		case TIMER3A:
		TCCR3A|=(1<<WGM30)|(1<<COM3A1);
		TCCR3B|=(1<<WGM32)|(1<<CS32)|(1<<CS31);
		break;
		
		case TIMER3B:
		TCCR3A|=(1<<WGM30)|(1<<COM3B1);
		TCCR3B|=(1<<WGM32)|(1<<CS32)|(1<<CS31);
		break;
		
		default:
		break;
		
	}
}

void setPwmDutyCycle(uint8_t dutyCycle, uint8_t timerNo )
{
	
	switch(timerNo)
	{
		case TIMER0 :
			OCR0 = dutyCycle;
			break;
		
		case TIMER1A:
			OCR1A = dutyCycle;
			break;
		
		case TIMER1B:
			OCR1B = dutyCycle;
			break;
		
		case TIMER2:
			OCR2 = dutyCycle;
			break;
		
		case TIMER3A:
			OCR3A = dutyCycle;
			break;
			
		case TIMER3B:
			OCR3B = dutyCycle;	
			break;
		
		default:
			break;
		
	}
}


void turnOffPWM(uint8_t timer)
{
	
	
	switch(timer)
	{
		case TIMER0 :
		TCCR0&=3;//for millis
		break;
		
		case TIMER1A:
		CLR_BIT(TCCR1A ,COM1A1);
		TCCR1B|=(1<<WGM12)|(1<<CS10);
		break;
		
		case TIMER1B:
		CLR_BIT(TCCR1A ,COM1B1);
		TCCR1B|=(1<<WGM12)|(1<<CS10);
		break;
		
		case TIMER2:
		TCCR2 &= 0x03;
		break;
		
		case TIMER3A:
		CLR_BIT(TCCR3A ,COM3A1);
		TCCR3B|=(1<<WGM32)|(1<<CS30);
		break;
		
		case TIMER3B:
		CLR_BIT(TCCR3A ,COM3B1);
		TCCR3B|=(1<<WGM32)|(1<<CS30);
		break;
		
		default:
		break;
		
	}
}


