/*
 * _1sheelds_functions.c
 *
 * Created: 24/12/2013 08:53:17 م
 *  Author: HP
 */ 
#include "gpio.h"
#include "pwm.h"
#include "mapping162.h"


void pinMode(uint8 pin , uint8 pinMode)
{
	uint8 bit = digitalPinToBitMask(pin);
	uint8 port = digitalPinToPort(pin);
	t_SetPortCfg cfg;
	
	if(port == NOT_A_PIN)
	{
		return;
	}
	
	
	//cfg.initValue=0x00;
	//SET_BIT((cfg.initValue),bit);
	cfg.pID = portModeRegister(port);
	
	if(pinMode == INPUT)
	{
		//cfg.Portdir &= (~(1 << bit)); 
		CLR_BIT((cfg.Portdir),bit);
		//cfg.Portdir=0x00;
	}
	else
	{		
		SET_BIT((cfg.Portdir),bit);
		//cfg.Portdir =0xff;//|= (1 << bit);
		//cfg.Portdir=0xff;
	}
	
	GPIO_Cfg(&cfg);
}

uint8  digitalRead(uint8 pin)
{
	uint8 bit = digitalPinToBitMask(pin);
	uint8 port = digitalPinToPort(pin);
	t_SetPortCfg cfg;
	
	if(port == NOT_A_PIN)
	{
		return 0;
	}
	
	cfg.pID = portModeRegister(port);
	
	return GPIO_getPin(cfg.pID,bit);
			
}

void   digitalWrite(uint8 pin, uint8 value)
{
	uint8 bit = digitalPinToBitMask(pin);
	uint8 port = digitalPinToPort(pin);
	t_SetPortCfg cfg;
	
	if(port == NOT_A_PIN)
	{
		//return;
	}
	
	cfg.pID = portModeRegister(port);
	
	GPIO_setPin(value,cfg.pID,bit);
}


void analogWrite(uint8 pin, uint16 val)
{
	uint8 timer = 0xff;
	pinMode(pin, OUTPUT);
		
	timer = digitalPinToTimer(pin);	 
	pwm_Setup(timer);
	pwm_SetDutyCycle(val, timer);	
}