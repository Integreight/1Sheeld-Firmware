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
#ifndef GPIO_H_
#define GPIO_H_
#include "pwm.h"


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
/**
 * @brief Reads the value of a certain digital pin
 * @param pinNumber pin number
 * @return current state of the pin 
 */
uint8  digitalRead(uint8);
/**
 * @brief Output a value to a digital pin
 * @param pinNumber pin number
 * @param value HIGH or LOW
 * @return None 
 */
void   digitalWrite(uint8, uint8);
/**
 * @brief Sets the pin whether it's Input, Output or PWM
 * @param pinNumber pin number
 * @param pinMode Mode (INPUT/OUTPUT/PWM)
 * @return None 
 */
void   setPinMode(uint8 , uint8);
/**
 * @brief Sets the duty cycle of a PWM pin.
 * @param pinNumber pwm pin number
 * @param val duty cycle 0 --> 255
 * @return None 
 */
void   analogWrite(uint8, int);
/**
 * @brief output a value to the whole port 
 * @param port port number 
 * @param value the value of the whole port to be output
 * @param bitmask masks the port to protect some pins and to retain their values 
 * @return None  
 */
void   writePort(byte, byte, byte);
/**
 * @brief reads the whole port value 
 * @param port port number to be red 
 * @param bitmask masks the port to protect some pins and to retain their values 
 * @return None  
 */
uint8  readPort(byte, byte);
/**
 * @brief Sets all unused pins to be Output 
 * @param None
 * @return  None  
 */
void setUnusedPinsAsOutput();

#endif /* GPIO_H_ */