#include "pwm.h"

void initPwm(uint8_t timer)
{
	switch(timer)
	{
		// make 64 prescalar for all timers except timer1=16
		case TIMER_0A :
		TCCR0A=(1<<WGM00)|(1<<COM0A1);
		TCCR0B=(1<<CS00)|(1<<CS01);
		break;
		
		case TIMER_0B:
		TCCR0A=(1<<WGM00)|(1<<COM0B1);
		TCCR0B=(1<<CS00)|(1<<CS01);
		break;
		
		case TIMER_1A:
		TCCR1A=(1<<WGM10)|(1<<COM1A1);
		TCCR1B=(1<<CS10)|(1<<CS11);
		break;
		
		case TIMER_1B:
		TCCR1A=(1<<WGM10)|(1<<COM1B1);
		TCCR1B=(1<<CS10)|(1<<CS11);
		break;
		
		case TIMER_2A:
		TCCR2A=(1<<WGM20)|(1<<COM2A1);
		TCCR2B=(1<<CS22);
		break;
		
		case TIMER_2B:
		TCCR2A=(1<<WGM20)|(1<<COM2B1);
		TCCR2B=(1<<CS22);
		break;
		
		case TIMER_4A:
		TCCR4A=(1<<WGM40)|(1<<COM4B1);
		TCCR4B=(1<<CS10)|(1<<CS11);
		break;
		default:
		break;
		
	}
}

void setPwmDutyCycle(uint8_t dutyCycle, uint8_t timerNo )
{
	
	switch(timerNo)
	{
		case TIMER_0A :
			OCR0A = dutyCycle;
			break;
		
		case TIMER_0B:
			OCR0B = dutyCycle;
			break;
		
		case TIMER_1A:
			OCR1AL = dutyCycle;
			break;
		
		case TIMER_1B:
			OCR1BL = dutyCycle;
			break;
		
		case TIMER_2A:
			OCR2A = dutyCycle;
			break;
			
		case TIMER_2B:
			OCR2B = dutyCycle;	
			break;
		
		case TIMER_4A:
			OCR4AL = dutyCycle;
			break;
		default:
			break;
		
	}
}


void turnOffPWM(uint8_t timer)
{
	
	switch(timer)
	{
		case TIMER_0A :
		CLR_BIT(TCCR0A ,COM0A1);
		break;
		
		case TIMER_0B:
		CLR_BIT(TCCR0A ,COM0B1);
		break;
		
		case TIMER_1A:
		CLR_BIT(TCCR1A ,COM1A1);
		break;
		
		case TIMER_1B:
		CLR_BIT(TCCR1A ,COM1B1);
		break;
		
		case TIMER_2A:
		CLR_BIT(TCCR2A ,COM2A1);
		break;
		
		case TIMER_2B:
		CLR_BIT(TCCR2A ,COM2B1);
		break;
		
		case TIMER_4A:
		CLR_BIT(TCCR4A ,COM4A1);
		break;
		default:
		break;
		
	}
}


