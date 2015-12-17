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

boolean	arduinoStopped;
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
/**
 * @brief Setup the millis timers  
 * @param None
 * @return  None  
 */
void setupMillisTimers();
/**
 * @brief Setup the LEDs as UART communication indicator  
 * @param None
 * @return  None  
 */
void setupUartLeds();
/**
 * @brief Send a frame to Arduino that the app is disconected  
 * @param None
 * @return  None  
 */
void sendFrameToArduino();
/**
 * @brief Send a frame to Arduino to stop sending Data  
 * @param None
 * @return  None  
 */
void sendArduinoToStopData();
/**
 * @brief Send a frame to Arduino to start sending Data  
 * @param None
 * @return  None  
 */
void sendArduinoToSendData();
/**
 * @brief Check if there's data in Arduino Buffer  
 * @param None
 * @return  None  
 */
void checkDataAvailabilityInRx0Buffer();
/**
 * @brief Check if the Arduino Buffer is Empty  
 * @param None
 * @return  None  
 */
void checkArduinoRx0BufferSpace();
/**
 * @brief Initializes the Hardware peripherals and setup for communications  
 * @param None
 * @return  None  
 */
void initialization();
/**
 * @brief Grab Time using Millis timer function 
 * @param None
 * @return  None  
 */
void catchTimeForSomeVariables();
/**
 * @brief Checks digitalPorts status  
 * @param None
 * @return  None  
 */
void checkDigitalPinStatus();
/**
* @brief Process data coming from application.
* @param None.
* @return None.
*/
void processDataFromApp();
/**
* @brief Check if application responded to the Bluetooth reset request.
* @param None.
* @return None.
*/
void checkBluetoothResetResponse();
/**
* @brief Check if app responded as alive and is still connected.
* @param None.
* @return None.
*/
void checkAppConnection();
/**
* @brief Send Data to the application in 20bytes frame each 15ms.
* @param None.
* @return None.
*/
void sendDataToApp();
/**
* @brief Check digital ports pin state equality.
* @param oldPort state,newPort state,number of pins.
* @return None.
*/
int checkPortStateEquality(byte * oldPort ,byte * newPort,byte numberOfPins);
/**
* @brief Put digital ports values in the 20byte buffer to be sent.
* @param None.
* @return None.
*/
void fillBufferWithPinStates(byte * portArray,byte portNumber);
#endif /* 1SHEELDS_FUNCTIONS_H_ */

