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
	isPulseInEnabled =0;
	stringPosition=0;
	isUartStringStarted=0;
	systemReset();
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
		/*case STRING_DATA:
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
		break;*/
		default:
		sysexCallback(storedInputData[0], sysexBytesRead - 1, storedInputData + 1);
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
				analogWriteCallback(multiByteChannel,
					(storedInputData[0] << 7)
					+ storedInputData[1]);
				
				break;

				case DIGITAL_MESSAGE:
				digitalWriteCallback(multiByteChannel,
					(storedInputData[0] << 7)
					+ storedInputData[1]);
				
				break;
				case SET_PIN_MODE:
				setPinModeCallback(storedInputData[1], storedInputData[0]);
				break;

				case REPORT_DIGITAL:
				reportDigitalCallback(multiByteChannel,storedInputData[0]);
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

/*// generic callbacks
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
*/
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

  systemResetCallback();
  
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
	//UartTx1((byte)value);
	sendValueAsTwo7bitBytes(value);
	endSysex();

}



/*==============================================================================
 * FUNCTIONS
 *============================================================================*/


void FirmataClass::outputPort(byte portNumber, byte portValue, byte forceSend)
{
  // pins not configured as INPUT are cleared to zeros
  portValue = portValue & portConfigInputs[portNumber];
  // only send if the value is different than previously sent
  if(forceSend || previousPINs[portNumber] != portValue) {
    Firmata.sendDigitalPort(portNumber, portValue);
    previousPINs[portNumber] = portValue;
  }
  
}

/* -----------------------------------------------------------------------------
 * check all the active digital inputs for change of state, then add any events
 * to the Serial output queue using Serial.print() */
void FirmataClass::checkDigitalInputs(void)
{
  /* Using non-looping code allows constants to be given to readPort().
   * The compiler will apply substantial optimizations if the inputs
   * to readPort() are compile-time constants. */
  if (TOTAL_PORTS > 0 && reportPINs[0]) outputPort(0, readPort(0, portConfigInputs[0]), false);
  if (TOTAL_PORTS > 1 && reportPINs[1]) outputPort(1, readPort(1, portConfigInputs[1]), false);
  if (TOTAL_PORTS > 2 && reportPINs[2]) outputPort(2, readPort(2, portConfigInputs[2]), false);
  if (TOTAL_PORTS > 3 && reportPINs[3]) outputPort(3, readPort(3, portConfigInputs[3]), false);
  if (TOTAL_PORTS > 4 && reportPINs[4]) outputPort(4, readPort(4, portConfigInputs[4]), false);
  if (TOTAL_PORTS > 5 && reportPINs[5]) outputPort(5, readPort(5, portConfigInputs[5]), false);
  if (TOTAL_PORTS > 6 && reportPINs[6]) outputPort(6, readPort(6, portConfigInputs[6]), false);
  if (TOTAL_PORTS > 7 && reportPINs[7]) outputPort(7, readPort(7, portConfigInputs[7]), false);
  if (TOTAL_PORTS > 8 && reportPINs[8]) outputPort(8, readPort(8, portConfigInputs[8]), false);
  if (TOTAL_PORTS > 9 && reportPINs[9]) outputPort(9, readPort(9, portConfigInputs[9]), false);
  if (TOTAL_PORTS > 10 && reportPINs[10]) outputPort(10, readPort(10, portConfigInputs[10]), false);
  if (TOTAL_PORTS > 11 && reportPINs[11]) outputPort(11, readPort(11, portConfigInputs[11]), false);
  if (TOTAL_PORTS > 12 && reportPINs[12]) outputPort(12, readPort(12, portConfigInputs[12]), false);
  if (TOTAL_PORTS > 13 && reportPINs[13]) outputPort(13, readPort(13, portConfigInputs[13]), false);
  if (TOTAL_PORTS > 14 && reportPINs[14]) outputPort(14, readPort(14, portConfigInputs[14]), false);
  if (TOTAL_PORTS > 15 && reportPINs[15]) outputPort(15, readPort(15, portConfigInputs[15]), false);
}

// -----------------------------------------------------------------------------
/* sets the pin mode to the correct state and sets the relevant bits in the
 * two bit-arrays that track Digital I/O and PWM status
 */
void FirmataClass::setPinModeCallback(byte pin, int mode)
{
  if (IS_PIN_DIGITAL(pin)) {
    if (mode == INPUT) {
      portConfigInputs[pin/8] |= (1 << (pin & 7));
    } else {
      portConfigInputs[pin/8] &= ~(1 << (pin & 7));
    }
  }
  pinState[pin] = 0;
  switch(mode) {
  case INPUT:
    if (IS_PIN_DIGITAL(pin)) {
      pinMode(pin, INPUT); // disable output driver
      digitalWrite(pin, LOW); // disable internal pull-ups
      pinConfig[pin] = INPUT;
    }
    break;
  case OUTPUT:
    if (IS_PIN_DIGITAL(pin)) {
      digitalWrite(pin, LOW); // disable PWM
      pinMode(pin, OUTPUT);
      pinConfig[pin] = OUTPUT;

    }
    break;
  case PWM:
    if (IS_PIN_PWM(pin)) {
      pinMode(pin, OUTPUT);
      analogWrite(pin, 0);
      pinConfig[pin] = PWM;
    }
    break;
  default:
    Firmata.sendString("Unknown pin mode"); // TODO: put error msgs in EEPROM
  }
  // TODO: save status to EEPROM here, if changed
}

void FirmataClass::analogWriteCallback(byte pin, int value)
{
	if (pin < TOTAL_PINS) {
	/*	switch(pinConfig[pin]) {
			
			case PWM:*/
			if (IS_PIN_PWM(pin))
			analogWrite(pin, value);
			
			pinState[pin] = value;
			//break;
		//}
	}
}

void FirmataClass::digitalWriteCallback(byte port, int value)
{
	byte pin, lastPin, mask=1, pinWriteMask=0;

	if (port < TOTAL_PORTS) {
		// create a mask of the pins on this port that are writable.
		lastPin = port*8+8;
		if (lastPin > TOTAL_PINS) lastPin = TOTAL_PINS;
		for (pin=port*8; pin < lastPin; pin++) {
			// do not disturb non-digital pins (eg, Rx & Tx)
			if (IS_PIN_DIGITAL(pin)) {
				// only write to OUTPUT and INPUT (enables pullup)
				// do not touch pins in PWM, ANALOG, SERVO or other modes
				if (pinConfig[pin] == OUTPUT || pinConfig[pin] == INPUT) {
					pinWriteMask |= mask;
					pinState[pin] = ((byte)value & mask) ? 1 : 0;
				}
			}
			mask = mask << 1;
		}
		writePort(port, (byte)value, pinWriteMask);
	}
}

void FirmataClass::reportDigitalCallback(byte port, int value)
{
	if (port < TOTAL_PORTS) {
		reportPINs[port] = (byte)value;
	}

}

/*==============================================================================
 * SYSEX-BASED commands
 *============================================================================*/

void FirmataClass::sysexCallback(byte command, byte argc, byte *argv)
{
           byte mode;
           byte slaveAddress;
           byte slaveRegister;
           byte data;
           byte newData [argc/2];
  unsigned int  delayTime; 
           int  s16DataCounter;
		       
		   
		        
  switch(command) {
	  case EXTENDED_ANALOG:
	  if (argc > 1) {
		  int val = argv[1];
		  if (argc > 2) val |= (argv[2] << 7);
		  if (argc > 3) val |= (argv[3] << 14);
		  analogWriteCallback(argv[0], val);
	  }
	  break;
/* sami's
      case SYSEX_UART:
	  for ( s16DataCounter = 0; s16DataCounter < argc; s16DataCounter+=2) // run over and over
	  {
		  newData[s16DataCounter/2]=(argv[s16DataCounter]|(argv[s16DataCounter+1]<<7));
		  UartTx0(newData[s16DataCounter/2]);
	  }
	  break;
	 */ 
	  case CAPABILITY_QUERY:
	  UartTx1(START_SYSEX);
	  UartTx1(CAPABILITY_RESPONSE);
	  for (byte pin=0; pin < TOTAL_PINS; pin++) {
		  if (IS_PIN_DIGITAL(pin)) 
		  {
			  UartTx1((byte)INPUT);			  
			  UartTx1(1);
			  UartTx1((byte)OUTPUT);
			  UartTx1(1);
		  }
	      if (IS_PIN_PWM(pin)) {
		      UartTx1(PWM);
		      UartTx1(8);
	      }
		  UartTx1(127);
	  }
	  UartTx1(END_SYSEX);
	  break;
	
	 case PIN_STATE_QUERY:
	 if (argc > 0) {
		 byte pin=argv[0];
		 UartTx1(START_SYSEX);
		 UartTx1(PIN_STATE_RESPONSE);
		 UartTx1(pin);
		 if (pin < TOTAL_PINS) {
			 UartTx1((byte)pinConfig[pin]);
			 UartTx1((byte)pinState[pin] & 0x7F);
			 if (pinState[pin] & 0xFF80) UartTx1((byte)(pinState[pin] >> 7) & 0x7F);
			 if (pinState[pin] & 0xC000) UartTx1((byte)(pinState[pin] >> 14) & 0x7F);
		 }
		 UartTx1(END_SYSEX);
	 }
	 break;
	  case UART_COMMAND:  
	  {
		  if(argv[0]==UART_BEGIN)
		  {
			  if(argv[1]==0x00) BAUD_RATE=1200;
			 // else  if(argv[1]==0x01) BAUD_RATE =2400;
			 // else  if(argv[1]==0x02) BAUD_RATE =4800;
			  else  if(argv[1]==0x03) BAUD_RATE =BAUD_9600;
			  else  if(argv[1]==0x04) BAUD_RATE =BAUD_14400;
			  else  if(argv[1]==0x05) BAUD_RATE =BAUD_19200;
			  else  if(argv[1]==0x06) BAUD_RATE =BAUD_28800;
			  else  if(argv[1]==0x07) BAUD_RATE =BAUD_38400;
			  else  if(argv[1]==0x08) BAUD_RATE =BAUD_57600;
			  else  if(argv[1]==0x09) BAUD_RATE =BAUD_115200;
			  else BAUD_RATE =57600;
			  UartInit(0,BAUD_RATE);

		  }
		  else if(argv[0]==UART_END) UartEnd(0);
		  
	  }break;
	case UART_DATA:
	{
		byte newData [argc/2];
		for (int i = 0; i < argc; i+=2) // run over and over
		{
			newData[i/2]=(argv[i]|(argv[i+1]<<7));
			UartTx0(newData[i/2]);
		}
	}
	break;  // todo later
	/*case PULSE_IN_INIT:  //0x86
	{
		delete(convertPWM);
		byte pin_no;
		if(argv[0]==0x03)
		pin_no=3;
		else  if(argv[0]==0x05)
		pin_no =5;
		
		convertPWM   = new  ConvertPWM(pin_no); // make ana object of ConvertPWM with pin 8 to read pulse in
		
		if(argv[1]==0x00)
		isPulseInEnabled =1;
		else  if(argv[1]==0x04)
		isPulseInEnabled =0;
		
	}break; */ 
	}
}
void FirmataClass::systemResetCallback()
{	
	for (byte i=0; i < TOTAL_PORTS; i++) {
		reportPINs[i] = false;      // by default, reporting off
		portConfigInputs[i] = 0;	// until activated
		previousPINs[i] = 0;
	}
	
	for (byte i=0; i < TOTAL_PINS; i++)
    {
		setPinModeCallback(i, OUTPUT);
	}
}

// make one instance for the user to use
FirmataClass Firmata ;
