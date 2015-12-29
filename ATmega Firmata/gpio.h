/*

  Project:       1Sheeld Firmware
  File:          gpio.h

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
  
  Date:          2014.5

*/

/**
 * @file gpio.h
 * @brief GPIO driver for configuring and dealing with GPIOs on ATmega162 
 * @author Integreight
 * @version 1.1
 */

#include "macros.h"


#ifndef GPIO_H_
#define GPIO_H_
#include"sys.h"


typedef struct
{
	uint8 portInReg;//pin
	uint8 portDirReg;//ddrc
	uint8 portOutReg;//portc
	
}t_stPort;

typedef struct
{
	byte Portdir;	
	unsigned int  pID;	
}t_SetPortCfg;

/**
 * @brief Sets the registers for a pin in a certain port to be an output.
 * @param *cfg pointer to the t_SetPortCfg structure .
 * @return None. 
 */
void setPinModeOutput(t_SetPortCfg* cfg, uint8 bit);
/**
 * @brief Sets the registers for a pin in a certain port to be an input.
 * @param *cfg pointer to the t_SetPortCfg structure .
 * @return None. 
 */
void setPinModeInput(t_SetPortCfg* cfg, uint8 bit);
/**
 * @brief Gets a certain value from pin of certain port.
 * @param pid the port id.
 * @param pinNum pin number.
 * @return None. 
 */
uint8 getPinValue(unsigned int pid,uint8 pinNum);
/**
 * @brief Sets a certain value to a pin of certain port.
 * @param data data to be put on the pin.
 * @param pid the port id.
 * @param pinNum pin number.
 * @return None. 
 */
void setPinValue(uint8 data,unsigned int pid,uint8 pinNum);

#endif /* GPIO_H_ */