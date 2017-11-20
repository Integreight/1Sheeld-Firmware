#include "gpio.h"

void setPinModeOutput(t_SetPortCfg* cfg, uint8_t bit)
{
	SET_BIT(cfg->Portdir,bit);
	t_stPort* stport=(t_stPort *)cfg->pID; //make the pointer points to the Port Registers in memory
	stport->portDirReg |= cfg->Portdir;
}

void setPinModeInput(t_SetPortCfg* cfg, uint8_t bit)
{
	t_stPort* stport=(t_stPort *)cfg->pID;  //make the pointer points to the Port Registers in memory
	cfg->Portdir=stport->portDirReg;
	CLR_BIT(cfg->Portdir,bit);
	stport->portDirReg &= cfg->Portdir;
}

uint8_t getPinValue(uint16_t pid,uint8_t pinNum)
{
	t_stPort* stport;
	
	
	stport=(t_stPort *)pid;
	return ((stport->portInReg)&(1<<pinNum));
}

void setPinValue(uint8_t data,uint16_t pid,uint8_t pinNum)
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

void setPinMode(uint8_t pin , uint8_t pinMode)
{
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
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

uint8_t  digitalRead(uint8_t pin)
{
	uint8_t timer = digitalPinToTimer(pin);
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	t_SetPortCfg cfg;
	
	if (timer != NOT_ON_TIMER) turnOffPWM(timer);

	if(port == NOT_A_PIN)
	{
		return 0;
	}
	
	cfg.pID = (uint16_t)portModeRegister(port);
	
	return getPinValue(cfg.pID,bit);
	
}

void   digitalWrite(uint8_t pin, uint8_t value)
{
	uint8_t timer = digitalPinToTimer(pin);
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	t_SetPortCfg cfg;

	if (timer != NOT_ON_TIMER) turnOffPWM(timer);
	
	if(port == NOT_A_PORT)
	{
		return;
	}
	
	cfg.pID = (uint16_t)portModeRegister(port);
	
	setPinValue(value,cfg.pID,bit);
}

void analogWrite(uint8_t pin, int16_t val)
{
	uint8_t timer = 0xff;
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
		setPwmDutyCycle((uint8_t)val, timer);

	}
	
}

uint8_t readPort(uint8_t port, uint8_t bitmask)
{
	uint8_t out=0, pin=port*8;
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

void writePort(uint8_t port, uint8_t value, uint8_t bitmask)
{
	uint8_t pin=port*8;
	
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
	for (int16_t i =25;i<35;i++)
	setPinMode(i,OUTPUT);
}