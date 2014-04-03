/*
 * firmata.h
 *
 * Created: 26/1/2014 2:31:25 PM
 *  Author: iMustafa
 */ 


#ifndef FIRMATA_H_
#define FIRMATA_H_

#include "sys.h"
#include "1sheelds_functions.h"
#include "mapping162.h"


/* Version numbers for the protocol.  The protocol is still changing, so these
 * version numbers are important.  This number can be queried so that host
 * software can test whether it will be compatible with the currently
 * installed firmware. */
#define VERSION_LOW   0xff 
#define VERSION_HIGH   0 
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



class FirmataClass
{
	public:
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
	FirmataClass();
	void begin();
	void write(unsigned char data);
	//void setFirmwareVersion(byte major, byte minor);  // see macro below
	//void setFirmwareNameAndVersion(const char *name, byte major, byte minor);
	/* serial receive handling */
	int available(void);
	void processInput(void);
	void processUart0Input(void);
	/* serial send handling */
	void sendDigitalPort(byte portNumber, int portData);
	void sendString(byte command, const char* string);
	void sendSysex(byte command, byte bytec, byte* bytev);
	void sendSysexDataByte(byte command, int value);
	void outputPort(byte portNumber, byte portValue, byte forceSend);
	void checkDigitalInputs(void);
	void setPinModeCallback(byte pin, int mode);
	void analogWriteCallback(byte pin, int value);
	void digitalWriteCallback(byte port, int value);
	void reportDigitalCallback(byte port, int value);
	void sysexCallback(byte command, byte argc, byte *argv);
	void systemResetCallback();
    private:
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

	/* private methods ------------------------------ */
	void processSysexMessage(void);
	void systemReset(void);
	void sendValueAsTwo7bitBytes(int value);
	void startSysex(void);
	void endSysex(void);
	void forceHardReset();
};

extern FirmataClass Firmata;

/*==============================================================================
 * MACROS
 *============================================================================*/

/* shortcut for setFirmwareNameAndVersion() that uses __FILE__ to set the
 * firmware name.  It needs to be a macro so that __FILE__ is included in the
 * firmware source file rather than the library source file.
 */
#define setFirmwareVersion(x, y)   setFirmwareNameAndVersion(__FILE__, x, y)


#endif /* FIRMATA_H_ */