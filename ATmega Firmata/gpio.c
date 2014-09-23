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