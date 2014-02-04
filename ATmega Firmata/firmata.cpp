/*
 * firmata.cpp
 *
 * Created: 26/1/2014 2:30:59 PM
 *  Author: iMustafa
 */ 
#define F_CPU 16000000UL

#include "firmata.h"
#include "uart.h"
#include <util/delay.h>

#include "mapping162.h"

extern "C" {
	#include <string.h>
	#include <stdlib.h>
}

//******************************************************************************
//* Support Functions
//******************************************************************************

void FirmataClass::sendValueAsTwo7bitBytes(int value)
{
  UartTx1(value & 0b01111111); // LSB
  UartTx1(value >> 7 & 0b01111111); // MSB
}

void FirmataClass::startSysex(void)
{
  UartTx1(START_SYSEX);
}

void FirmataClass::endSysex(void)
{
  UartTx1(END_SYSEX);
}

//constructor 
FirmataClass::FirmataClass()
{
	firmwareVersionCount = 0;
	firmwareVersionVector = 0;
	systemReset();
	UartTx1('6');
}

//******************************************************************************
//* Public Methods
//******************************************************************************

/* begin method for overriding default serial bitrate */
void FirmataClass::begin(void)
{
	begin(57600);
}

/* begin method for overriding default serial bitrate */
void FirmataClass::begin(long speed)
{
	switch (speed)
	{
		case 9600:
		baudRate=BAUD_9600;
		break;
		
		case 14400:
		baudRate=BAUD_14400;
		break;
		
		case 19200:
		baudRate=BAUD_19200;
		break;
		
		case 28800:
		baudRate=BAUD_28800;
		break;
		
		case 38400:
		baudRate=BAUD_38400;
		
		case 57600:
		baudRate=BAUD_57600;
		break;
		
		case 76800:
		baudRate=BAUD_76800;
		break;
		
		case 115200:
		baudRate=BAUD_115200;
		break;
		
		default: break;
	}
	UartInit(1,baudRate);// 1 for rx1,tx1 

	//blinkVersion();  
	//printVersion();
	//printFirmwareVersion();
}

// output the protocol version message to the serial port
void FirmataClass::printVersion(void) {
	UartTx1(REPORT_VERSION);
	UartTx1(FIRMATA_MAJOR_VERSION);
	UartTx1(FIRMATA_MINOR_VERSION);
}

void FirmataClass::blinkVersion(void)
{
	// flash the pin with the protocol version
	pinMode(VERSION_BLINK_PIN,OUTPUT);
	pin13strobe(FIRMATA_MAJOR_VERSION, 40, 210);
	_delay_ms(250);
	pin13strobe(FIRMATA_MINOR_VERSION, 40, 210);
	_delay_ms(125);
}

void FirmataClass::printFirmwareVersion(void)
{
	byte i;

	if(firmwareVersionCount) { // make sure that the name has been set before reporting
		startSysex();
		UartTx1(REPORT_FIRMWARE);
		UartTx1(firmwareVersionVector[0]); // major version number
		UartTx1(firmwareVersionVector[1]); // minor version number
		for(i=2; i<firmwareVersionCount; ++i) {
			sendValueAsTwo7bitBytes(firmwareVersionVector[i]);
		}
		endSysex();
	}
}

void FirmataClass::setFirmwareNameAndVersion(const char *name, byte major, byte minor)
{
	const char *filename;
	char *extension;

	// parse out ".cpp" and "applet/" that comes from using __FILE__
	extension = strstr(name, ".cpp");
	filename = strrchr(name, '/') + 1; //points to slash, +1 gets to start of filename
	// add two bytes for version numbers
	if(extension && filename) {
		firmwareVersionCount = extension - filename + 2;
		} else {
		firmwareVersionCount = strlen(name) + 2;
		filename = name;
	}
	firmwareVersionVector = (byte *) malloc(firmwareVersionCount);
	firmwareVersionVector[firmwareVersionCount] = 0;
	firmwareVersionVector[0] = major;
	firmwareVersionVector[1] = minor;
	strncpy((char*)firmwareVersionVector + 2, filename, firmwareVersionCount - 2);
}

int FirmataClass::available(void)
{
	return serial1_Avilable();
}

void FirmataClass::processSysexMessage(void)
{
	switch(storedInputData[0]) { //first byte in buffer is command
		case REPORT_FIRMWARE:
		printFirmwareVersion();
		break;
		case STRING_DATA:
		if(currentStringCallback) {
			byte bufferLength = (sysexBytesRead - 1) / 2;
			char *buffer = (char*)malloc(bufferLength * sizeof(char));
			byte i = 1;
			byte j = 0;
			while(j < bufferLength) {
				buffer[j] = (char)storedInputData[i];
				i++;
				buffer[j] += (char)(storedInputData[i] << 7);
				i++;
				j++;
			}
			(*currentStringCallback)(buffer);
		}
		break;
		default:
		if(currentSysexCallback)
		(*currentSysexCallback)(storedInputData[0], sysexBytesRead - 1, storedInputData + 1);
	}
}

void FirmataClass::processInput(void)
{
	int inputData = UartRx1(); // this is 'int' to handle -1 when no data
	int command;
	
	// TODO make sure it handles -1 properly

	if (parsingSysex) {
		if(inputData == END_SYSEX) {
			//stop sysex byte
			parsingSysex = false;
			//fire off handler function
			processSysexMessage();
			} else {
			//normal data byte - add to buffer
			storedInputData[sysexBytesRead] = inputData;
			sysexBytesRead++;
		  }
		} else if( (waitForData > 0) && (inputData < 128) ) {
		waitForData--;
		storedInputData[waitForData] = inputData;
		if( (waitForData==0) && executeMultiByteCommand ) { // got the whole message
			switch(executeMultiByteCommand) {
				case ANALOG_MESSAGE:
				if(currentAnalogCallback) {
					(*currentAnalogCallback)(multiByteChannel,
					(storedInputData[0] << 7)
					+ storedInputData[1]);
				}
				break;

				case DIGITAL_MESSAGE:
				if(currentDigitalCallback) {
					(*currentDigitalCallback)(multiByteChannel,
					(storedInputData[0] << 7)
					+ storedInputData[1]);
				}
				break;
				case SET_PIN_MODE:
				if(currentPinModeCallback)
				(*currentPinModeCallback)(storedInputData[1], storedInputData[0]);
				break;

				case REPORT_DIGITAL:
				if(currentReportDigitalCallback)
				(*currentReportDigitalCallback)(multiByteChannel,storedInputData[0]);
				break;
			}
			executeMultiByteCommand = 0;
		 }
		} else {
		// remove channel info from command byte if less than 0xF0
		if(inputData < 0xF0) {
			command = inputData & 0xF0;
			multiByteChannel = inputData & 0x0F;
			} else {
			command = inputData;
			// commands in the 0xF* range don't use channel data
		}
		switch (command) {
			
			case DIGITAL_MESSAGE:
			case ANALOG_MESSAGE:
			case SET_PIN_MODE:
			waitForData = 2; // two data bytes needed
			executeMultiByteCommand = command;
			break;
			
			case REPORT_DIGITAL:
			waitForData = 1; // two data bytes needed
			executeMultiByteCommand = command;
			break;
			case START_SYSEX:
			parsingSysex = true;
			sysexBytesRead = 0;
			break;
			case SYSTEM_RESET:
			systemReset();
			UartTx1('5');
			break;
			case REPORT_VERSION:
			//Firmata.printVersion();
			break;
		}
	}
}

void FirmataClass::sendDigitalPort(byte portNumber, int portData)
{
	UartTx1(DIGITAL_MESSAGE | (portNumber & 0xF));
	UartTx1((byte)portData % 128); // Tx bits 0-6
	UartTx1(portData >> 7);  // Tx bits 7-13
}

void FirmataClass::sendSysex(byte command, byte bytec, byte* bytev)
{
	byte i;
	startSysex();
	UartTx1(command);
	for(i=0; i<bytec; i++) {
		sendValueAsTwo7bitBytes(bytev[i]);
	}
	endSysex();
}

void FirmataClass::sendString(byte command, const char* string)
{
	sendSysex(command, strlen(string), (byte *)string);
}


// send a string as the protocol string type
void FirmataClass::sendString(const char* string)
{
	sendString(STRING_DATA, string);
}

// Internal Actions/////////////////////////////////////////////////////////////

// generic callbacks
void FirmataClass::attach(byte command, callbackFunction newFunction)
{
	switch(command) {
		
		case DIGITAL_MESSAGE: currentDigitalCallback = newFunction; break;
		case ANALOG_MESSAGE : currentAnalogCallback = newFunction; break;
		case REPORT_DIGITAL: currentReportDigitalCallback = newFunction; break;
		case SET_PIN_MODE: currentPinModeCallback = newFunction; break;
	}
}

void FirmataClass::attach(byte command, systemResetCallbackFunction newFunction)
{
	switch(command) {
		case SYSTEM_RESET: currentSystemResetCallback = newFunction; break;
	}
}

void FirmataClass::attach(byte command, stringCallbackFunction newFunction)
{
	switch(command) {
		case STRING_DATA: currentStringCallback = newFunction; break;
	}
}

void FirmataClass::attach(byte command, sysexCallbackFunction newFunction)
{
	currentSysexCallback = newFunction;
}

void FirmataClass::detach(byte command)
{
	switch(command) {
		case SYSTEM_RESET: currentSystemResetCallback = NULL; break;
		case STRING_DATA: currentStringCallback = NULL; break;
		case START_SYSEX: currentSysexCallback = NULL; break;
		default:
		attach(command, (callbackFunction)NULL);
	}
}

//******************************************************************************
//* Private Methods
//******************************************************************************



// resets the system state upon a SYSTEM_RESET message from the host software
void FirmataClass::systemReset(void)
{
  byte i;

  waitForData = 0; // this flag says the next serial input will be data
  executeMultiByteCommand = 0; // execute this after getting multi-byte data
  multiByteChannel = 0; // channel data for multiByteCommands


  for(i=0; i<MAX_DATA_BYTES; i++) {
    storedInputData[i] = 0;
  }

  parsingSysex = false;
  sysexBytesRead = 0;

  if(currentSystemResetCallback)
    (*currentSystemResetCallback)();

  //flush(); //TODO uncomment when Firmata is a subclass of HardwareSerial
}

// =============================================================================
// used for flashing the pin for the version number
void FirmataClass::pin13strobe(int count, int onInterval, int offInterval)
{ /*
	byte i;
	pinMode(VERSION_BLINK_PIN, OUTPUT);
	for(i=0; i<count; i++) {
		_delay_ms(offInterval);
		digitalWrite(VERSION_BLINK_PIN, HIGH);
		_delay_ms(onInterval);
		digitalWrite(VERSION_BLINK_PIN, LOW);
	} */
}

void FirmataClass::sendSysexDataByte(byte command, int value){

	startSysex();
	UartTx1(command);
	sendValueAsTwo7bitBytes(value);
	endSysex();

}


// make one instance for the user to use
FirmataClass Firmata ;
