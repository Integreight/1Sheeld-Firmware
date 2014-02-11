/*
 * gpio.h
 *
 * Created: 27/09/2013 02:17:32 م
 *  Author: HP
 */ 


#ifndef GPIO_H_
#define GPIO_H_
#include"sys.h"


typedef enum {
	
	INPUT_GPIO=0x00,
	OUTPUT_GPIO=0xFF
}t_ePortDirection;

typedef struct
{
	uint8 portInReg;//pin
	uint8 portDirReg;//ddrc
	uint8 portOutReg;//portc
	
}t_stPort;

typedef enum {
	
	PinIn=0,
	PinOUT=1
}t_ePinDirection;



typedef struct
{
	byte Portdir;	
	uint8 initValue;
	unsigned int pID;
	t_ePinDirection pindir;	
}t_SetPortCfg;

void GPIO_Cfg(t_SetPortCfg* cfg);
void GPIO_CfgIN(t_SetPortCfg* cfg);
void GPIO_setPort(uint8 data,unsigned int pid);
uint8 GPIO_getPort(unsigned int pid);
uint8 GPIO_getPin(unsigned int pid,uint8 pinNum);
void GPIO_setPin(uint8 data,unsigned int pid,uint8 pinNum);

#endif /* GPIO_H_ */