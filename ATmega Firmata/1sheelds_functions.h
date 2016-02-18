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
#ifndef SHEELDS_FUNCTIONS_H_
#define SHEELDS_FUNCTIONS_H_

#include "firmata.h"

/**
 * @brief Send a frame to Arduino that the app is disconected  
 * @param None
 * @return  None  
 */
void sendFrameToArduino();
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
#ifdef IOS_VERSION
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
#endif // IOS_VERSION
#endif /* 1SHEELDS_FUNCTIONS_H_ */

