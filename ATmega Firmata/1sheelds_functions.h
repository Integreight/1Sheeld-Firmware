/*

  Project:       1Sheeld Firmware 
  File:          1sheelds_functions.h

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
                 
  Date:          2014.5

*/

/**
 * @file 1sheelds_functions.h
 * @brief This library contains an Arduino abstraction layer to mimic Arduino methods and a set of 1Sheeld related methods.
 * @author Integreight
 * @version 1.1
 */ 

#include "firmata.h"
#include "gpio.h"
#include "pwm.h"
#include "timers.h"
#include <util/delay.h>

#ifndef SHEELDS_FUNCTIONS_H_
#define SHEELDS_FUNCTIONS_H_

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
///**
// * @brief Reads the time of a pulse "either High or Low" on a certain pin
// * @param pinNumber digital pin number 
// * @param state the type of pulse to read "High or Low"
// * @return the length of the pulse or 0 if no pulse started before the timeout 
// */
//unsigned long pulseIn(uint8_t , uint8_t);
///**
// * @brief Reads a pulse "either High or Low" on a certain pin and return the equivalent pwm value
// * @param pinNumber digital pin number 
// * @return pulse value with range from 0 -->255
// */
//unsigned long readPWM (int);
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
 * @brief Get the number of bytes  available for reading from the serial0 port
 * @param None
 * @return the numbers of byte available to read  
 */
int getAvailableDataCountOnSerial0();
/**
 * @brief Get the number of bytes available for reading from the serial1 port
 * @param None
 * @return the numbers of byte available to read  
 */
int getAvailableDataCountOnSerial1();
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
void setUnusedPinsAsOutput();
/**
 * @brief Reset the Bluetooth using software  
 * @param None
 * @return  None  
 */
void resetBluetooth();

#endif /* 1SHEELDS_FUNCTIONS_H_ */

void setupMillisTimers();
void setupUartLeds();
void sendFrameToArduino();
void sendArduinoToStopData();
void sendArduinoToSendData();
void checkDataAvailabilityInRx0Buffer();
void checkArduinoRx0BufferSpace();
void initialization();
void catchTimeForSomeVariables();