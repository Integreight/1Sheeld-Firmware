/*

  Project:       1Sheeld Firmware 
  File:          firmata.h

  Compiler:      Arduino avr-gcc 4.3.2

  Author:        Integreight
                 
  Date:          2014.5

*/

/**
 * @file firmata.h
 * @brief This Library illustrates the firmata protocols functions   	
 * @author Integreight
 * @version 1.1
 */ 

#ifndef FIRMATA_H_
#define FIRMATA_H_

#include <string.h>
#include <stdlib.h>
#include "sys.h"
#include "1sheelds_functions.h"
#include "mapping162.h"
#include "uart.h"

/* Version numbers for the protocol.  The protocol is still changing, so these
 * version numbers are important.  This number can be queried so that host
 * software can test whether it will be compatible with the currently
 * installed firmware. */
#define VERSION_MINOR   1 
#define VERSION_MAJOR   1 
#define FIRMATA_BUGFIX_VERSION  1 // for bugfix releases
#define VERSION_BLINK_PIN 13 //blink led
#define MAX_DATA_BYTES 32 // max number of data bytes in non-Sysex messages

// message command bytes (128-255/0x80-0xFF)
#define DIGITAL_MESSAGE         0x90 // send data for a digital pin
#define ANALOG_MESSAGE          0xE0 // send data for an analog pin (or PWM)
#define REPORT_ANALOG           0xC0 // enable analog input by pin #
#define REPORT_DIGITAL          0xD0 // enable digital input by port pair
//
#define SET_PIN_MODE            0xF4 // set a pin to INPUT/OUTPUT/PWM/etc
//
#define REPORT_VERSION          0xF9 // report protocol version
#define SYSTEM_RESET            0xFF // reset from MIDI
//
#define START_SYSEX             0xF0 // start a MIDI Sysex message
#define END_SYSEX               0xF7 // end a MIDI Sysex message

// extended command set using sysex (0-127/0x00-0x7F)
/* 0x00-0x0F reserved for user-defined commands */
#define SERVO_CONFIG            0x70 // set max angle, minPulse, maxPulse, freq
#define STRING_DATA             0x71 // a string message with 14-bits per char
#define SHIFT_DATA              0x75 // a bitstream to/from a shift register
#define I2C_REQUEST             0x76 // send an I2C read/write request
#define I2C_REPLY               0x77 // a reply to an I2C read request
#define I2C_CONFIG              0x78 // config I2C settings such as delay times and power pins
#define EXTENDED_ANALOG         0x6F // analog write (PWM, Servo, etc) to any pin
#define PIN_STATE_QUERY         0x6D // ask for a pin's current mode and value
#define PIN_STATE_RESPONSE      0x6E // reply with pin's current mode and value
#define CAPABILITY_QUERY        0x6B // ask for supported modes and resolution of all pins
#define CAPABILITY_RESPONSE     0x6C // reply with supported modes and resolution
#define ANALOG_MAPPING_QUERY    0x69 // ask for mapping of analog to pin numbers
#define ANALOG_MAPPING_RESPONSE 0x6A // reply with mapping info
#define REPORT_FIRMWARE         0x79 // report name and version of the firmware
#define SAMPLING_INTERVAL       0x7A // set the poll rate of the main loop
#define SYSEX_NON_REALTIME      0x7E // MIDI Reserved for non-realtime messages
#define SYSEX_REALTIME          0x7F // MIDI Reserved for realtime messages
// these are DEPRECATED to make the naming more consistent
#define FIRMATA_STRING          0x71 // same as STRING_DATA
#define SYSEX_I2C_REQUEST       0x76 // same as I2C_REQUEST
#define SYSEX_I2C_REPLY         0x77 // same as I2C_REPLY
#define SYSEX_SAMPLING_INTERVAL 0x7A // same as SAMPLING_INTERVAL
#define FIRMATA_MUTE            0x64  // turn off firmata
#define FIRMATA_VERSION         0X63 // our firmata version
#define IS_ALIVE                0x62
#define RESET_BLUETOOTH         0x61          
#define RESET_MICRO             0x60
         
// pin modes
#define ANALOG                  0x02 // analog pin in analogInput mode
#define PWM                     0x03 // digital pin in PWM output mode
#define SHIFT                   0x05 // shiftIn/shiftOut mode

//uart 
#define UART_COMMAND 0x65
#define UART_DATA 0x66
#define PULSE_IN_INIT  0x67
#define PULSE_IN_DATA 0x68
#define REGISTER_NOT_SPECIFIED -1
#define STX 0x02
#define ETX 0x03
#define MAX_UART_STRING_SIZE 150
#define UART_BEGIN 0x01
#define UART_END 0x00

#define TOTAL_PORTS             5
#define TOTAL_PINS              20
#define SYSEX_UART              0x0A




	//variables declarations//
	uint16 BAUD_RATE;// for the hardware serial terminal
    byte isPulseInEnabled;
    byte pinPWM;
	/* digital input ports */
	byte reportPINs[TOTAL_PORTS];       // 1 = report this port, 0 = silence
	byte previousPINs[TOTAL_PORTS];     // previous 8 bits sent
	// PIN means PORT for input

	/* pins configuration */
	byte pinConfig[TOTAL_PINS];         // configuration of every pin
	byte portConfigInputs[TOTAL_PORTS]; // each bit: 1 = pin in INPUT, 0 = anything else
	int pinState[TOTAL_PINS];           // any value that has been written

	char incomingString[MAX_UART_STRING_SIZE];
	int stringPosition;
	char tempChar;
	char isUartStringStarted;
	uint8 muteFlag;
	//serial
	uint16 baudRate;
	/* firmware name and version */
	byte firmwareVersionCount;
	byte *firmwareVersionVector;
	/* input message handling */
	byte waitForData; // this flag says the next serial input will be data
	byte executeMultiByteCommand; // execute this after getting multi-byte data
	byte multiByteChannel; // channel data for multiByteCommands
	byte storedInputData[MAX_DATA_BYTES]; // multi-byte data
	/* sysex */
	boolean parsingSysex;
	int sysexBytesRead;
	//for bluetooth reset
	boolean responseFlag ;
	
	/**
	* @brief Begins the Firmata protocol.
	* @param None.
	* @return None. 
    */
	void begin();
	/**
	* @brief Sends a byte "data" using the Serial communication.
	* @param data bytes to be sent.
	* @return None. 
	*/
	void write(unsigned char data);
	//void setFirmwareVersion(byte major, byte minor);  // see macro below
	//void setFirmwareNameAndVersion(const char *name, byte major, byte minor);
	/**
	* @brief Checks the availability of data on the serial port.
	* @param None.
	* @return number of data on the serial buffer. 
	*/
	int available(void);
	/**
	* @brief process the input data coming from the android application.
	* @param None.
	* @return None. 
	*/
	void processInput(void);
	/**
	* @brief process the input data coming from the arduino.
	* @param None.
	* @return None. 
	*/
	void processUart0Input(void);
	/**
	* @brief Sending digital messages supported by firmata protocol.
	* @param portNumber port number to carry the data.
	* @param portData Data to be adjusted on the port.
	* @return None. 
	*/
	void sendDigitalPort(byte portNumber, int portData);
	/**
	* @brief Sending strings using Sysex message Format.
	* @param command Command type.
	* @param *string String to be sent.
	* @return None. 
	*/
	void sendString(byte command, const char* string);
	/**
	* @brief Supporting Sysex message format.
	* @param command Command type.
	* @param bytec Length of data will be sent.
	* @param *bytev Array carrying the data.
	* @return None. 
	*/
	void sendSysex(byte command, byte bytec, byte* bytev);
	/**
	* @brief Supports individual byte sending using Sysex message format .
	* @param command Command type.
	* @param value Data"byte" to be sent.
	* @return None. 
	*/
	void sendSysexDataByte(byte command, int value);
	/**
	* @brief Set the port with certain read values.
	* @param portNumber port number to carry data.
	* @param portValue Value to be adjusted on port.
	* @param forceSend Variable to controll the variation of values on port.
	* @return None. 
	*/
	void outputPort(byte portNumber, byte portValue, byte forceSend);
	/**
	* @brief Checks the values on ports
	* @param None.
	* @return None. 
	*/
	void checkDigitalInputs(void);
	/**
	* @brief sets the pin mode to the correct state and sets the relevant bits in the two bit-arrays that track Digital I/O and PWM status
	* @param pin number of pin to be set.
	* @param mode mode of the pin "Input/Output".
	* @return None. 
	*/
	void setPinModeCallback(byte pin, int mode);
	/**
	* @brief sets analog values to analog pins.
	* @param pin number of pin to be set.
	* @param value analog values to be set on the pin.
	* @return None. 
	*/
	void analogWriteCallback(byte pin, int value);
	/**
	* @brief sets digital values to pins.
	* @param pin number of pin to be set.
	* @param value digital value to be set on the pin.
	* @return None. 
	*/
	void digitalWriteCallback(byte port, int value);
	/**
	* @brief Report values on pins.
	* @param port number of port to be reported.
	* @param value digital value to be set on port.
	* @return None. 
	*/
	void reportDigitalCallback(byte port, int value);
	/**
	* @brief processing the Sysec base messages.
	* @param command Command type.
	* @param argc length of the data.
	* @param *argv Array of Sysex data.
	* @return None. 
	*/
	void sysexCallback(byte command, byte argc, byte *argv);
	/**
	* @brief Reset only Sysex based message variables.
	* @param None.
	* @return None. 
	*/
	void systemResetCallback();
	/**
	* @brief Sends a request to the android to reset system.
	* @param None.
	* @return None. 
	*/
	void requestBluetoothReset();
	/**
	* @brief Flag for the bluetooth request reset.
	* @param None.
	* @return boolean. 
	*/
	boolean getResponseFlag();
    /**
	* @brief Reset the flag for the bluetooth request reset.
	* @param state boolean.
	* @return None. 
	*/
	void setResponseFlag(boolean);

	/* private methods ------------------------------ */
	/**
	* @brief Process the Input Sysex messages.
	* @param None.
	* @return None. 
	*/
	void processSysexMessage(void);
	/**
	* @brief Resets the whole sytem.
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
	* @brief Encapsulate start sysex commands to farmes.
	* @param None.
	* @return None. 
	*/
	void startSysex(void);
	/**
	* @brief Encapsulate end sysex commands to farmes.
	* @param None.
	* @return None. 
	*/
	void endSysex(void);
	/**
	* @brief Enable watchdog timer to force a hard reset to system.
	* @param None.
	* @return None. 
	*/
	void forceHardReset();
	/**
	* @brief Send commands to android to print the version.
	* @param None.
	* @return None. 
	*/
	void printVersion();
#endif /* FIRMATA_H_ */