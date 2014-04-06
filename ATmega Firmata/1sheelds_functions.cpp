/*
 * _1sheelds_functions.c
 *
 * Created: 24/12/2013 08:53:17 م
 *  Author: HP
 */

#include "gpio.h"
#include "pwm.h"
#include "uart.h"
#include "mapping162.h"
#define F_CPU 7372800UL
#include <util/delay.h>

void pinMode(uint8 pin , uint8 pinMode)
{
	uint8 bit = digitalPinToBitMask(pin);
	uint8 port = digitalPinToPort(pin);
	t_SetPortCfg cfg;
		
	
	if(port == NOT_A_PIN) 
	{
		return;
	}
	
	
	
	cfg.pID =  portModeRegister(port);
	
	if(pinMode == INPUT)
	{
		t_stPort* stport=(t_stPort *)cfg.pID; //make the pointer points to the Port Registers in memory
    	cfg.Portdir=stport->portDirReg;
	    CLR_BIT(cfg.Portdir,bit);
	    GPIO_CfgIN(&cfg);
	}
	else 
	{		
		SET_BIT(cfg.Portdir,bit);
	    GPIO_Cfg(&cfg);
	}
	
}

unsigned long pulseIn(uint8_t pin, uint8_t state)
{

	uint8_t bit = digitalPinToBitMaskPWM(pin);
	uint8_t port = digitalPinToPort(pin);
	uint8_t stateMask = (state ?bit : 0);
	unsigned long width = 0; // keep initialization out of time critical area
	
	unsigned long numloops = 0;
	unsigned long maxloops = microsecondsToClockCycles(1000000L) / 16;
	
	while ((*portInputRegister(port) & bit) == stateMask)
	if (numloops++ == maxloops)
	return 0;

	while ((*portInputRegister(port) & bit) != stateMask)
	if (numloops++ == maxloops)		return 0;
	
	// wait for the pulse to stop
	while ((*portInputRegister(port) & bit) == stateMask) {
		if (numloops++ == maxloops)
		return 0;
		width++;
	}

	// convert the reading to microseconds. The loop has been determined
	// to be 20 clock cycles long and have about 16 clocks between the edge
	// and the start of the loop. There will be some error introduced by
	// the interrupt handlers.
	return clockCyclesToMicroseconds(width * 21 + 16);
}
unsigned long readPWM (int pin)
{
	double period =0;
	double duty =0;
	double fraction =0;
	period = (double)pulseIn(pin, HIGH)+ (double)pulseIn(pin, LOW);
	duty = (double)pulseIn(pin, HIGH);
	fraction =duty / period ;
	return ((floor)(fraction *255));

}
void turnOffPWM(uint8 timer)
{
	
	
	switch(timer)
	{
		case TIMER_00 :
		TCCR0=0;//for millis
		break;
		
		case TIMER_1A:
		CLR_BIT(TCCR1A ,COM1A1);
		TCCR1B|=(1<<WGM12)|(1<<CS10);
		break;
		
		case TIMER_1B:
		CLR_BIT(TCCR1A ,COM1B1);
		TCCR1B|=(1<<WGM12)|(1<<CS10);
		break;
		
		case TIMER_02:
		TCCR2 &= 0x03;
		break;
		
		case TIMER_3A:
		CLR_BIT(TCCR3A ,COM3A1);
		TCCR3B|=(1<<WGM32)|(1<<CS30);
		break;
		
		case TIMER_3B:
		CLR_BIT(TCCR3A ,COM3B1);
		TCCR3B|=(1<<WGM32)|(1<<CS30);
		break;
		
		default:
		break;
		
	}
}

uint8  digitalRead(uint8 pin)
{
	uint8 timer = digitalPinToTimer(pin);
	uint8 bit = digitalPinToBitMask(pin);
	uint8 port = digitalPinToPort(pin);
	t_SetPortCfg cfg;
	
	if (timer != NOT_ON_TIMER) turnOffPWM(timer);

	if(port == NOT_A_PIN)
	{
		return 0;
	}
	
	cfg.pID = (unsigned int)portModeRegister(port);
	
	return GPIO_getPin(cfg.pID,bit);
			
}

void resetBluetooth()
{
	//bt reset
	SET_BIT(DDRE,0);
	SET_BIT(PORTE,0);
	_delay_ms(5);
	CLR_BIT(PORTE,0);
}
void   digitalWrite(uint8 pin, uint8 value)
{
	uint8 timer = digitalPinToTimer(pin);
	uint8 bit = digitalPinToBitMask(pin);
	uint8 port = digitalPinToPort(pin);
	t_SetPortCfg cfg;

	if (timer != NOT_ON_TIMER) turnOffPWM(timer);
	
	if(port == NOT_A_PIN)
	{
		//return;
	}
	
	cfg.pID = (unsigned int)portModeRegister(port);
	
	GPIO_setPin(value,cfg.pID,bit);
}


void analogWrite(uint8 pin, int val)
{
	uint8 timer = 0xff;
	pinMode(pin, OUTPUT);
	
	if ((val == 0) || (val <0))
	{
		digitalWrite(pin, LOW);

	}
	else if ((val > 255) || (val == 255))
	{
		digitalWrite(pin, HIGH);

	}
	else
	{
		timer = digitalPinToTimer(pin);
		pwm_Setup(timer);
		pwm_SetDutyCycle((uint8)val, timer);

	}
	
}


uint8 readPort(byte port, byte bitmask)
{
	unsigned char out=0, pin=port*8;
	if ((IS_PIN_DIGITAL(pin+0)) && (bitmask & 0x01) && ( digitalRead(pin+0))) out |= 0x01;
	if ((IS_PIN_DIGITAL(pin+1)) && (bitmask & 0x02) && ( digitalRead(pin+1))) out |= 0x02;
	if ((IS_PIN_DIGITAL(pin+2)) && (bitmask & 0x04) && ( digitalRead(pin+2))) out |= 0x04;
	if ((IS_PIN_DIGITAL(pin+3)) && (bitmask & 0x08) && ( digitalRead(pin+3))) out |= 0x08;
	if ((IS_PIN_DIGITAL(pin+4)) && (bitmask & 0x10) && ( digitalRead(pin+4))) out |= 0x10;
	if ((IS_PIN_DIGITAL(pin+5)) && (bitmask & 0x20) && ( digitalRead(pin+5))) out |= 0x20;
	if ((IS_PIN_DIGITAL(pin+6)) && (bitmask & 0x40) && ( digitalRead(pin+6))) out |= 0x40;
	if ((IS_PIN_DIGITAL(pin+7)) && (bitmask & 0x80) && ( digitalRead(pin+7))) out |= 0x80;
	return out;
}

void writePort(byte port, byte value, byte bitmask)
{
	byte pin=port*8;
	
	if (port==0)
	{
       bitmask&=0xfc; // don't touch uart pins (0,1)!!
    }
	else if (port==2)
	{
	   bitmask&=0x7f; // don't touch uart pins (23)!!
	}
	else if (port==3)
	{
	   bitmask&=0xfe; // don't touch uart pins (24)!!
	}

	if ((bitmask & 0x01)) digitalWrite((pin+0), (value & 0x01));
	if ((bitmask & 0x02)) digitalWrite((pin+1), (value & 0x02));
	if ((bitmask & 0x04)) digitalWrite((pin+2), (value & 0x04));
	if ((bitmask & 0x08)) digitalWrite((pin+3), (value & 0x08));
	if ((bitmask & 0x10)) digitalWrite((pin+4), (value & 0x10));
	if ((bitmask & 0x20)) digitalWrite((pin+5), (value & 0x20));
	if ((bitmask & 0x40)) digitalWrite((pin+6), (value & 0x40));
	if ((bitmask & 0x80)) digitalWrite((pin+7), (value & 0x80));
}

void unusedPinsAsOutput()
{
	pinMode(22,OUTPUT); 
	for (int i =25;i<35;i++)
		pinMode(i,OUTPUT);
}
uint16_t serial0_Avilable()
{
	
	
	return getuartRx0Flag();
	
}


int serial1_Avilable()
{
	
	
	return getuartRx1Flag();
	
}
