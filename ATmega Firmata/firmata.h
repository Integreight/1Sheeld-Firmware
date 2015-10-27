/*

  Project:       1Sheeld Firmware 
  File:          firmata.h

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
                 
  Date:          2014.5

*/

/**
 * @file firmata.h
 * @brief This library handles all of the messages related to Firmata protocol.   	
 * @author Integreight
 * @version 1.1
 */ 

#ifndef FIRMATA_H_
#define FIRMATA_H_

#include <string.h>
#include <stdlib.h>
#include "sys.h"
#include "1sheelds_functions.h"
#include "atmega162_mapping.h"
#include "uart.h"

#define ONESHEELD_MINOR_FIRMWARE_VERSION   4 
#define ONESHEELD_MAJOR_FIRMWARE_VERSION   1 
#define MAX_DATA_BYTES 32 // max number of data bytes in non-Sysex messages

#define PIN_STATE_QUERY         0x6D // ask for a pin's current mode and value
#define PIN_STATE_RESPONSE      0x6E // reply with pin's current mode and value

// message command bytes (128-255/0x80-0xFF)
#define DIGITAL_MESSAGE         0x90 // send data for a digital pin
#define ANALOG_MESSAGE          0xE0 // send data for an analog pin (or PWM)
#define REPORT_ANALOG           0xC0 // enable analog input by pin #
#define REPORT_DIGITAL          0xD0 // enable digital input by port pair
#define SET_PIN_MODE            0xF4 // set a pin to INPUT/OUTPUT/PWM/etc
#define REPORT_VERSION          0xF9 // report protocol version
#define SYSTEM_RESET            0xFF // reset from MIDI
#define START_SYSEX             0xF0 // start a MIDI Sysex message
#define END_SYSEX               0xF7 // end a MIDI Sysex message
#define FIRMATA_MUTE            0x64  // turn off firmata
#define FIRMATA_VERSION         0X63 // our firmata version
#define IS_ALIVE                0x62
#define RESET_BLUETOOTH         0x61          
#define RESET_MICRO             0x60
#define REPORT_INPUT_PINS       0X5F

// pin modes
#define ANALOG                  0x02 // analog pin in analogInput mode
#define PWM                     0x03 // digital pin in PWM output mode
#define SHIFT                   0x05 // shiftIn/shiftOut mode

//uart 
#define UART_COMMAND 0x65
#define UART_DATA 0x66
//#define PULSE_IN_INIT  0x67
//#define PULSE_IN_DATA 0x68
#define REGISTER_NOT_SPECIFIED -1
#define STX 0x02
#define ETX 0x03
#define UART_BEGIN 0x01
#define UART_END 0x00

#define IS_PIN_REPORTING_ENABLED 1
#define TOTAL_PORTS             5
#define TOTAL_PINS              20

//variables declarations//
//byte isPulseInEnabled;
//byte pinPWM;
/* digital input ports */
byte reportPINs[TOTAL_PORTS];       // 1 = report this port, 0 = silence
byte previousPINs[TOTAL_PORTS];     // previous 8 bits sent
/* pins configuration */
byte pinConfig[TOTAL_PINS];         // configuration of every pin
byte portConfigInputs[TOTAL_PORTS]; // each bit: 1 = pin in INPUT, 0 = anything else
int pinState[TOTAL_PINS];           // any value that has been written
uint8 muteFlag;
/* input message handling */
byte waitForData; // this flag says the next serial input will be data
byte executeMultiByteCommand; // execute this after getting multi-byte data
byte multiByteChannel; // channel data for multiByteCommands
byte storedInputData[MAX_DATA_BYTES]; // multi-byte data
//to send frames of 20bytes each 15ms
byte UartTx1Buffer[20];
unsigned long sentFramesMillis;
uint8 txBufferIndex;
boolean uart1WriteFlag;
/* sysex */
boolean parsingSysex;
int sysexBytesRead;
//for bluetooth reset
boolean rbResetResponseFlag;
boolean isAliveResponseFlag;
boolean notAliveFrameSent;	
/**
* @brief Initialize Firmata protocal variables.
* @param None.
* @return None. 
*/
void initFirmata();
/**
* @brief Send a byte using the serial communication.
* @param data byte to be sent.
* @return None. 
*/
void write(unsigned char data);
/**
* @brief Checks the availability of data in the serial buffer.
* @param None.
* @return number of data bytes in the serial buffer. 
*/
int available(void);
/**
* @brief process the data coming from 1Sheeld android app.
* @param None.
* @return None. 
*/
void processInput(void);
/**
* @brief process the input data coming from the Arduino.
* @param None.
* @return None. 
*/
void processUart0Input(void);
/**
* @brief Sending digital post status using Firmata's digital message.
* @param portNumber port number.
* @param portData Data to be put on the port.
* @return None. 
*/
void sendDigitalPort(byte portNumber, int portData);
/**
* @brief Send an array of bytes using Firmata's sysex message format.
* @param command Command byte.
* @param bytec Length of data to be sent.
* @param *bytev Array carrying the data.
* @return None. 
*/
void sendSysex(byte command, byte bytec, byte* bytev);
/**
* @brief Send an individual byte using Firmata's sysex message format.
* @param command Command byte.
* @param value Data"byte" to be sent.
* @return None. 
*/
void sendSysexDataByte(byte command, int value);
/**
* @brief set the port before sending.
* @param portNumber port number.
* @param portValue data to be put on the port.
* @param forceSend boolean to send the port even if it didn't change.
* @return None. 
*/
void outputPort(byte portNumber, byte portValue, byte forceSend);
/**
* @brief Checks the values on all the digital inputs of 1Sheeld board.
* @param None.
* @return None. 
*/
void checkDigitalInputs(void);
/**
* @brief sets the pin mode to the correct state and sets the relevant bits in the two bit-arrays that track Digital I/O and PWM status when a pin mode message is received.
* @param pin number of pin to be set.
* @param mode mode of the pin either INPUT, OUTPUT, ANALOG or PWM.
* @return None. 
*/
void setPinModeCallback(byte pin, int mode);
/**
* @brief sets duty cycle to of an analog (PWM) pin when an analog message is received.
* @param pin number of pin to be set.
* @param duty cycle to be set on the pin.
* @return None. 
*/
void analogWriteCallback(byte pin, int value);
/**
* @brief sets digital values to pins when a digital message is received.
* @param pin number of pin to be set.
* @param value digital value to be set on the pin.
* @return None. 
*/
void digitalWriteCallback(byte port, int value);
/**
* @brief enables/disables reporting of changes of a digital input port when a digital reporting message is received.
* @param port number of port to be reported.
* @param value a value that determine the enabling or disabling.
* @return None. 
*/
void reportDigitalCallback(byte port, int value);
/**
* @brief act upon the processed sysex messages.
* @param command command byte.
* @param argc length of the data.
* @param *argv Array of the enclosed sysex data.
* @return None. 
*/
void sysexCallback(byte command, byte argc, byte *argv);
/**
* @brief resets 1Sheeld pins to their default values and reset firmata values.
* @param None.
* @return None. 
*/
void systemResetCallback();
/**
* @brief Sends a request to the app to reset the Bluetooth.
* @param None.
* @return None. 
*/
void requestBluetoothReset();
/**
* @brief Flag ti know if the app responeded to the Bluetooth reset request.
* @param None.
* @return boolean. 
*/
boolean getBtResponseFlag();
/**
* @brief sets the flag for Bluetooth reset request.
* @param state boolean.
* @return None. 
*/
void setBtResponseFlag(boolean);

/* private methods ------------------------------ */
/**
* @brief Process sysex messages.
* @param None.
* @return None. 
*/
void processSysexMessage(void);
/**
* @brief Resets the whole firmata and init its variables.
* @param None.
* @return None. 
*/
void systemReset(void);
/**
* @brief Send byte as two 7bit bytes.
* @param None.
* @return None. 
*/
void sendValueAsTwo7bitBytes(int value);
/**
* @brief sends start sysex byte.
* @param None.
* @return None. 
*/
void startSysex(void);
/**
* @brief sends end sysex byte.
* @param None.
* @return None. 
*/
void endSysex(void);
/**
* @brief forse the mc to reset itself using the watchdog timer.
* @param None.
* @return None. 
*/
void forceHardReset();
/**
* @brief send the firmware version to the app.
* @param None.
* @return None. 
*/
void printVersion();

void sendIsAlive();
void setIsAliveResponseFlag(boolean state);
boolean getIsAliveResponseFlag();
boolean getIsAliveFrameNotSent();
void setIsAliveFrameNotSent(boolean state);
#endif /* FIRMATA_H_ */