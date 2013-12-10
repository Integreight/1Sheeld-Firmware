/*
 * gpio.c
 *
 * Created: 30/09/2013 08:28:06 م
 *  Author: HP
 */ 
#include "gpio.h"
#include "CommonMacros.h"

void GPIO_Cfg(t_SetPortCfg* cfg)
{
	t_stPort* stport;      //pointer to port structure
	stport=(t_stPort *)cfg->pID; //make the pointer points to the Port Registers in memory
		
	//cfg->pID=cfg->dir;
	stport->portDirReg=cfg->Portdir;
	
	//DDRC=cfg->dir;
	if(cfg->Portdir==OUTPUT)
		stport->portOutReg=cfg->initValue;
		
		//PORTC=cfg->initValue;	
}

void GPIO_setPort(uint8 data,t_ePortID pid)
{
	t_stPort* stport;
	stport=(t_stPort *)pid;
	
	stport->portOutReg=data;
		
}


uint8 GPIO_getPort(t_ePortID pid)
{
	t_stPort* stport;
	stport=(t_stPort *)pid;
	
	return stport->portInReg;
}
uint8 GPIO_getPin(t_ePortID pid,uint8 pinNum)
{
	t_stPort* stport;
	stport=(t_stPort *)pid;
	return (stport->portInReg&(1<<pinNum));
	
}

void GPIO_setPin(uint8 data,t_ePortID pid,uint8 pinNum)
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
		break;
		
	}	
}