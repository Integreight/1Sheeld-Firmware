/*

  Project:       1Sheeld Firmware 
  File:          1sheelds_functions.h

  Compiler:      Arduino avr-gcc 4.3.2

  Author:        Integreight
                 
  Date:          2014.5

*/

/**
 * @file 1sheelds_functions.h
 * @brief This Library illustrates 1Sheeld's Functions used by the whole libraries 	
 * @author Integreight
 * @version 1.1
 */ 

#include "sys.h"
#include "gpio.h"
#include "pwm.h"
#include "uart.h"
#include "mapping162.h"
#include <util/delay.h>

#ifndef SHEELDS_FUNCTIONS_H_
#define SHEELDS_FUNCTIONS_H_

/**
 * @brief Function to read value from a certain digital pin
 * @param pin number of the  pin selected
 * @return current state of the pin 
 */
uint8  digitalRead(uint8);
/**
 * @brief Function to insert a value on the selected pin
 * @param pin number of the  pin selected
 * @return None 
 */
void   digitalWrite(uint8, uint8);
/**
 * @brief Function to set the pin whether it's Input or Output
 * @param pin number of the  pin selected
 * @param pinMode Mode "Direction" to be set (Input/Output)
 * @return None 
 */
void   pinMode(uint8 , uint8);
/**
 * @brief Function to set the pin whether it's Input or Output
 * @param pin number of the  pin "analog pin" selected 
 * @param val give a certain analog signal value to the selected pin "analog pin"
 * @return None 
 */
void   analogWrite(uint8, int);
/**
 * @brief Reads a pulse "either High or Low" on a selected pin
 * @param pin number of the  pin "analog pin" selected 
 * @param state the type of pulse state to read "High or Low"
 * @return the length of the pulse or 0 if no pulse started before the timeout 
 */
unsigned long pulseIn(uint8_t , uint8_t);
/**
 * @brief Reads a pulse "either High or Low" on a selected pin
 * @param pin number of the  pin "analog pin" selected 
 * @return pulse value with range from 0"Low"-->255"High" 
 */
unsigned long readPWM (int);
/**
 * @brief Adjust a certain value to the whole port 
 * @param port port number to be adjusted 
 * @param value the value of the whole port 
 * @param bitmask masks the port to protect special pins and to retain their values 
 * @return None  
 */
void   writePort(byte, byte, byte);
/**
 * @brief read the whole port value 
 * @param port port number to be red 
 * @param bitmask masks the port to protect special pins and to retain their values 
 * @return None  
 */
uint8  readPort(byte, byte);
/**
 * @brief Get the number of bytes (characters) available for reading from the serial0 port
 * @param None
 * @return the numbers of byte available to read  
 */
int serial0_Avilable();
/**
 * @brief Get the number of bytes (characters) available for reading from the serial1 port
 * @param None
 * @return the numbers of byte available to read  
 */
int serial1_Avilable();
/**
 * @brief turning of the certain timers used by the library 
 * @param timer number of the timer to be turned off  
 * @return  None  
 */
void turnOffPWM(uint8);
/**
 * @brief Sets all unused pins to be Output 
 * @param None
 * @return  None  
 */
void unusedPinsAsOutput();
/**
 * @brief Reset the bluetooth using software  
 * @param None
 * @return  None  
 */
void resetBluetooth();

#endif /* 1SHEELDS_FUNCTIONS_H_ */