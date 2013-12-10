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
	
	INPUT=0x00,
	OUTPUT=0xFF
}t_ePortDirection;

typedef struct
{
	uint8 portInReg;//pin
	uint8 portDirReg;//ddrc
	uint8 portOutReg;//portc
	
}t_stPort;

//Registers addresses in the memory 
typedef enum {
	
	A=0x39,
	B=0x36,
	C=0x33,
	D=0x30
}t_ePortID;

typedef enum {
	
	PinIn=0,
	PinOUT=1
}t_ePinDirection;



typedef struct
{
	t_ePortDirection Portdir;	
	uint8 initValue;
	t_ePortID pID;
	t_ePinDirection pindir;
	
	
	
}t_SetPortCfg;

void GPIO_Cfg(t_SetPortCfg* cfg);
void GPIO_setPort(uint8 data,t_ePortID pid);
uint8 GPIO_getPort(t_ePortID pid);
uint8 GPIO_getPin(t_ePortID pid,uint8 pinNum);
void GPIO_setPin(uint8 data,t_ePortID pid,uint8 pinNum);

#endif /* GPIO_H_ */