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
	uint8_t portInReg;//pin
	uint8_t portDirReg;//ddrc
	uint8_t portOutReg;//portc
	
}t_stPort;

typedef struct
{
	uint8_t Portdir;	
	uint16_t  pID;	
}t_SetPortCfg;

/**
 * @brief Sets the registers for a pin in a certain port to be an output.
 * @param *cfg pointer to the t_SetPortCfg structure .
 * @return None. 
 */
void setPinModeOutput(t_SetPortCfg* cfg, uint8_t bit);
/**
 * @brief Sets the registers for a pin in a certain port to be an input.
 * @param *cfg pointer to the t_SetPortCfg structure .
 * @return None. 
 */
void setPinModeInput(t_SetPortCfg* cfg, uint8_t bit);
/**
 * @brief Gets a certain value from pin of certain port.
 * @param pid the port id.
 * @param pinNum pin number.
 * @return None. 
 */
uint8_t getPinValue(uint16_t pid,uint8_t pinNum);
/**
 * @brief Sets a certain value to a pin of certain port.
 * @param data data to be put on the pin.
 * @param pid the port id.
 * @param pinNum pin number.
 * @return None. 
 */
void setPinValue(uint8_t data,uint16_t pid,uint8_t pinNum);
/**
 * @brief Reads the value of a certain digital pin
 * @param pinNumber pin number
 * @return current state of the pin 
 */
uint8_t  digitalRead(uint8_t);
/**
 * @brief Output a value to a digital pin
 * @param pinNumber pin number
 * @param value HIGH or LOW
 * @return None 
 */
void   digitalWrite(uint8_t, uint8_t);
/**
 * @brief Sets the pin whether it's Input, Output or PWM
 * @param pinNumber pin number
 * @param pinMode Mode (INPUT/OUTPUT/PWM)
 * @return None 
 */
void   setPinMode(uint8_t , uint8_t);
/**
 * @brief Sets the duty cycle of a PWM pin.
 * @param pinNumber pwm pin number
 * @param val duty cycle 0 --> 255
 * @return None 
 */
void   analogWrite(uint8_t, int16_t);
/**
 * @brief output a value to the whole port 
 * @param port port number 
 * @param value the value of the whole port to be output
 * @param bitmask masks the port to protect some pins and to retain their values 
 * @return None  
 */
void   writePort(uint8_t, uint8_t, uint8_t);
/**
 * @brief reads the whole port value 
 * @param port port number to be red 
 * @param bitmask masks the port to protect some pins and to retain their values 
 * @return None  
 */
uint8_t  readPort(uint8_t, uint8_t);
/**
 * @brief Sets all unused pins to be Output 
 * @param None
 * @return  None  
 */
void setUnusedPinsAsOutput();

#endif /* GPIO_H_ */