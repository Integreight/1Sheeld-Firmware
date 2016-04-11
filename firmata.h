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
#include "stdint.h"
#include "uart.h"
#include "gpio.h"
#include "eeprom.h"

#define ONESHEELD_MINOR_FIRMWARE_VERSION   6 
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
#define BLUETOOTH_RENAMING		0x5E
#define TESTING_FRAME			0x5D
#define QUERY_UART0_BAUD_RATE	0x5C
#define SET_UART0_BAUD_RATE		0x5B

// pin modes
#define ANALOG                  0x02 // analog pin in analogInput mode
#define PWM                     0x03 // digital pin in PWM output mode
#define SHIFT                   0x05 // shiftIn/shiftOut mode

//uart 
#define UART_COMMAND 0x65
#define UART_DATA 0x66
#define REGISTER_NOT_SPECIFIED -1
#define STX 0x02
#define ETX 0x03
#define UART_BEGIN 0x01
#define UART_END 0x00

//eeprom address
#define CURRENT_UART0_BAUD_RATE_EEPROM_ADDRESS	0

#define IS_PIN_REPORTING_ENABLED 1
#define TOTAL_PORTS             5
#define TOTAL_PINS              20

#define BLUETOOH_RESET_RESPONSE_INTERVAL 500UL
#define FRAME_GAP	15UL
#define APP_ISALIVE_RESPONSE_INTERVAL	2000UL


/* digital input ports */
uint8_t reportPINs[TOTAL_PORTS];       // 1 = report this port, 0 = silence
uint8_t previousPINs[TOTAL_PORTS];     // previous 8 bits sent
/* pins configuration */
uint8_t pinConfig[TOTAL_PINS];         // configuration of every pin
uint8_t portConfigInputs[TOTAL_PORTS]; // each bit: 1 = pin in INPUT, 0 = anything else
int16_t pinState[TOTAL_PINS];           // any value that has been written
uint8_t muteFirmata;
/* input message handling */
uint8_t waitForData; // this flag says the next serial input will be data
uint8_t executeMultiByteCommand; // execute this after getting multi-byte data
uint8_t multiByteChannel; // channel data for multiByteCommands
uint8_t storedInputData[MAX_DATA_BYTES]; // multi-byte data
extern const uint8_t PROGMEM atNameArray[7];
uint8_t testAnswer;
uint32_t bluetoothResponseMillis;
uint32_t newMillis;
uint32_t isAliveMillis;
#ifdef PLUS_BOARD
/* Buffering data for IOS version */
uint32_t sentFramesMillis;
uint8_t txBufferIndex;
uint8_t UartTx1Buffer[20];	//to send frames of 20bytes each 15ms
uint8_t digitalPort0array[2];
uint8_t digitalPort1array[2];
uint8_t digitalPort2array[2];
uint8_t oldDigitalPort0array[2];
uint8_t oldDigitalPort1array[2];
uint8_t oldDigitalPort2array[2];
uint8_t toggelingIndicator;
uint8_t	resendDigitalPort;
uint8_t resendIsAlive;
uint8_t resendPrintVersion;
uint8_t	arduinoStopped;
uint8_t resendTestingAnswer;
uint8_t resendCurrentBaudRate;
#endif
/* sysex */
uint8_t parsingSysex;
int16_t sysexBytesRead;
//for bluetooth reset
uint8_t bluetoothResetResponded;
uint8_t isAppResponded;
uint8_t notAliveSentToArduino;
	
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
void write(uint8_t data);
/**
* @brief Checks the availability of data in the serial buffer.
* @param None.
* @return number of data bytes in the serial buffer. 
*/
int16_t available(void);
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
void sendDigitalPort(uint8_t portNumber, int16_t portData);
/**
* @brief Send an array of bytes using Firmata's sysex message format.
* @param command Command byte.
* @param bytec Length of data to be sent.
* @param *bytev Array carrying the data.
* @return None. 
*/
void sendSysex(uint8_t command, uint8_t bytec, uint8_t* bytev);
/**
* @brief set the port before sending.
* @param portNumber port number.
* @param portValue data to be put on the port.
* @param forceSend boolean to send the port even if it didn't change.
* @return None. 
*/
void outputPort(uint8_t portNumber, uint8_t portValue, uint8_t forceSend);
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
void setPinModeCallback(uint8_t pin, int16_t mode);
/**
* @brief sets duty cycle to of an analog (PWM) pin when an analog message is received.
* @param pin number of pin to be set.
* @param duty cycle to be set on the pin.
* @return None. 
*/
void analogWriteCallback(uint8_t pin, int16_t value);
/**
* @brief sets digital values to pins when a digital message is received.
* @param pin number of pin to be set.
* @param value digital value to be set on the pin.
* @return None. 
*/
void digitalWriteCallback(uint8_t port, int16_t value);
/**
* @brief enables/disables reporting of changes of a digital input port when a digital reporting message is received.
* @param port number of port to be reported.
* @param value a value that determine the enabling or disabling.
* @return None. 
*/
void reportDigitalCallback(uint8_t port, int16_t value);
/**
* @brief act upon the processed sysex messages.
* @param command command byte.
* @param argc length of the data.
* @param *argv Array of the enclosed sysex data.
* @return None. 
*/
void sysexCallback(uint8_t command, uint8_t argc, uint8_t *argv);
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
void sendValueAsTwo7bitBytes(int16_t value);
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
* @brief force the Micro-controller to reset itself using the watchdog timer.
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
/**
* @brief Report digital ports status to app.
* @param None.
* @return None.
*/
void reportDigialPorts();
/**
* @brief Sends Frame each 500ms to check if app is connected.
* @param None.
* @return None.
*/
void sendIsAlive();
/**
 * @brief Reset the Bluetooth using software  
 * @param None
 * @return  None  
 */
void resetBluetooth();
/**
 * @brief Send AT+NAME command to bluetooth to rename
 * @param None
 * @return  None  
 */
void sendATNameCommand();
/**
 * @brief Send Confirmation when bluetooth is renamed
 * @param None
 * @return  None  
 */
void sendBluetoothRenameConfirmation();
/**
 * @brief Send testing answer "SDK testing"
 * @param None
 * @return  None  
 */
void sendAnswerToApplication();
/**
 * @brief send the current UART0 baudRate to applicaiton
 * @param None
 * @return  None  
 */
void getCurrentUart0BaudRate();
/**
 * @brief Resend the unsent data.
 * @param None
 * @return  None  
 */
void checkNeededDataToResend();
#endif /* FIRMATA_H_ */