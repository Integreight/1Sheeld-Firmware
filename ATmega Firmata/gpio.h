/*

  Project:       1Sheeld Firmware
  File:          gpio.h

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
  
  Date:          2014.5

*/

/**
 * @file gpio.h
 * @brief This Library deals with the input/output registers  	
 * @author Integreight
 * @version 1.1
 */

#include "CommonMacros.h"


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
 * @brief Sets the registers as an Output.
 * @param *cfg pointer to the t_SetPortCfg structure .
 * @return None. 
 */
void GPIO_Cfg(t_SetPortCfg* cfg);
/**
 * @brief Reset the registers to Input.
 * @param *cfg pointer to the t_SetPortCfg structure .
 * @return None. 
 */
void GPIO_CfgIN(t_SetPortCfg* cfg);
/**
 * @brief Sets a certain value to the Port.
 * @param data data to be adjusted on the port.
 * @param pid the port number "id".
 * @return None. 
 */
void GPIO_setPort(uint8 data,unsigned int pid);
/**
 * @brief Gets a certain value from the Port.
 * @param pid the port number "id".
 * @return None. 
 */
uint8 GPIO_getPort(unsigned int pid);
/**
 * @brief Gets a certain value from pin of certain port.
 * @param pid the port number "id".
 * @param pinNum pin number.
 * @return None. 
 */
uint8 GPIO_getPin(unsigned int pid,uint8 pinNum);
/**
 * @brief Sets a certain value to a pin of certain port.
 * @param data data to be adjusted to6 the pin.
 * @param pid the port number "id".
 * @param pinNum pin number.
 * @return None. 
 */
void GPIO_setPin(uint8 data,unsigned int pid,uint8 pinNum);

#endif /* GPIO_H_ */