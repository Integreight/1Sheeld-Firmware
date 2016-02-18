/*

  Project:       1Sheeld Firmware 
  File:          gpio.cpp

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
                 
  Date:          2014.5

*/

#include "gpio.h"

void setPinModeOutput(t_SetPortCfg* cfg, uint8 bit)
{
	SET_BIT(cfg->Portdir,bit);
	t_stPort* stport=(t_stPort *)cfg->pID; //make the pointer points to the Port Registers in memory		
	stport->portDirReg |= cfg->Portdir;
}

void setPinModeInput(t_SetPortCfg* cfg, uint8 bit)
{
	t_stPort* stport=(t_stPort *)cfg->pID;  //make the pointer points to the Port Registers in memory
	cfg->Portdir=stport->portDirReg;
    CLR_BIT(cfg->Portdir,bit);
    stport->portDirReg &= cfg->Portdir;
}

uint8 getPinValue(unsigned int pid,uint8 pinNum)
{
	t_stPort* stport;
	
	
	stport=(t_stPort *)pid;
	return ((stport->portInReg)&(1<<pinNum));
}

void setPinValue(uint8 data,unsigned int pid,uint8 pinNum)
{
	t_stPort* stport;
	
	
	stport=(t_stPort *)pid;
	
	switch(data)
	{
		case 0:
			CLR_BIT((stport->portOutReg),pinNum);
		break;
		
		case 1:
			SET_BIT((stport->portOutReg),pinNum);
		break;
		
		default:
		    SET_BIT((stport->portOutReg),pinNum);//19/1
		break;
		
	}	
}

void setPinMode(uint8 pin , uint8 pinMode)
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
		setPinModeInput(&cfg, bit);
	}
	else
	{
		setPinModeOutput(&cfg, bit);
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
	
	return getPinValue(cfg.pID,bit);
	
}

void   digitalWrite(uint8 pin, uint8 value)
{
	uint8 timer = digitalPinToTimer(pin);
	uint8 bit = digitalPinToBitMask(pin);
	uint8 port = digitalPinToPort(pin);
	t_SetPortCfg cfg;

	if (timer != NOT_ON_TIMER) turnOffPWM(timer);
	
	if(port == NOT_A_PORT)
	{
		return;
	}
	
	cfg.pID = (unsigned int)portModeRegister(port);
	
	setPinValue(value,cfg.pID,bit);
}

void analogWrite(uint8 pin, int val)
{
	uint8 timer = 0xff;
	setPinMode(pin, OUTPUT);
	
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
		initPwm(timer);
		setPwmDutyCycle((uint8)val, timer);

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

void setUnusedPinsAsOutput()
{
	setPinMode(22,OUTPUT);
	for (int i =25;i<35;i++)
	setPinMode(i,OUTPUT);
}