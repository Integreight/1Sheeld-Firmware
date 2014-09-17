/*

  Project:       1Sheeld Firmware 
  File:          firmata.cpp

  Compiler:      Arduino avr-gcc 4.3.2

  Author:        Integreight
                 
  Date:          2014.5

*/

#include "firmata.h"
#include <avr/wdt.h>
#include <util/delay.h>

extern "C" {
	#include <string.h>
	#include <stdlib.h>
}

//******************************************************************************
//* Support Functions
//******************************************************************************

void FirmataClass::write(unsigned char data)
{
	if (muteFlag==0)
	{
		UartTx1(data);
	}
}
void FirmataClass::sendValueAsTwo7bitBytes(int value)
{
  write(value & 0b01111111); // LSB
  write(value >> 7 & 0b01111111); // MSB
}

void FirmataClass::startSysex(void)
{
  write(START_SYSEX);
}

void FirmataClass::endSysex(void)
{
  write(END_SYSEX);
}

//constructor 
FirmataClass::FirmataClass()
{
	firmwareVersionCount = 0;
	firmwareVersionVector = 0;
	isPulseInEnabled =0;
	stringPosition=0;
	isUartStringStarted=0;
	muteFlag=0;
	systemReset();
}

//******************************************************************************
//* Public Methods
//******************************************************************************

void FirmataClass::forceHardReset()
{
	cli();
	// disable interrupts
	wdt_enable(WDTO_15MS);
	// enable watchdog
	while(1);
	// wait for watchdog to reset processor
}

/* begin method for overriding default serial bitrate */
void FirmataClass::begin()
{
	UartInit(1);// 1 for rx1,tx1 with
}

int FirmataClass::available(void)
{
	return serial1_Avilable();
}

void FirmataClass::processSysexMessage(void)
{

		sysexCallback(storedInputData[0], sysexBytesRead - 1, storedInputData + 1);
}

void FirmataClass::processUart0Input(){
	uint16_t availableData=serial0_Avilable();
	if(availableData>0){
		byte arr[availableData];
		for(uint16_t i=0;i<availableData;i++){
			arr[i]=UartRx0();
		}
		Firmata.sendSysex(UART_DATA,availableData,arr);
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
			case REPORT_VERSION:
			printVersion();
			break;
			case SYSTEM_RESET:
			systemReset();
			break;

		}
	}
}

void FirmataClass::sendDigitalPort(byte portNumber, int portData)
{
	write(DIGITAL_MESSAGE | (portNumber & 0xF));
	write((byte)portData % 128); // Tx bits 0-6
	write(portData >> 7);  // Tx bits 7-13
}

void FirmataClass::sendSysex(byte command, byte bytec, byte* bytev)
{
	byte i;
	startSysex();
	write(command);
	for(i=0; i<bytec; i++) {
		sendValueAsTwo7bitBytes(bytev[i]);
	}
	endSysex();
}

void FirmataClass::sendString(byte command, const char* string)
{
	sendSysex(command, strlen(string), (byte *)string);
}

void FirmataClass::requestBluetoothReset()
{
	write(START_SYSEX);
	write(RESET_BLUETOOTH);
	write(END_SYSEX);
}

bool FirmataClass::getResponseFlag()
{
	return responseFlag;
}

void FirmataClass::setResponseFlag(bool state)
{
	responseFlag=state;
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
  muteFlag=0;

  for(i=0; i<MAX_DATA_BYTES; i++) {
    storedInputData[i] = 0;
  }

  parsingSysex = false;
  sysexBytesRead = 0;
  responseFlag=false;
  systemResetCallback();
 
}

void FirmataClass::printVersion()
{
	write(REPORT_VERSION);
	write(VERSION_MINOR);
	write(VERSION_MAJOR);
}
// =============================================================================
void FirmataClass::sendSysexDataByte(byte command, int value){

	startSysex();
	write(command);
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
  if (reportPINs[0]) outputPort(0, readPort(0, portConfigInputs[0]), false);
  if (reportPINs[1]) outputPort(1, readPort(1, portConfigInputs[1]), false);
  if (reportPINs[2]) outputPort(2, readPort(2, portConfigInputs[2]), false);
  if (reportPINs[3]) outputPort(3, readPort(3, portConfigInputs[3]), false);
  if (reportPINs[4]) outputPort(4, readPort(4, portConfigInputs[4]), false);
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
    Firmata.sendString(STRING_DATA,"Unknown pin mode"); // TODO: put error msgs in EEPROM
  }
  // TODO: save status to EEPROM here, if changed
}

void FirmataClass::analogWriteCallback(byte pin, int value)
{
	if (pin < TOTAL_PINS) {

			if (IS_PIN_PWM(pin))
			analogWrite(pin, value);
			pinState[pin] = value;

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

		   
		        
  switch(command) {
	  case EXTENDED_ANALOG:
	  if (argc > 1) {
		  int val = argv[1];
		  if (argc > 2) val |= (argv[2] << 7);
		  if (argc > 3) val |= (argv[3] << 14);
		  analogWriteCallback(argv[0], val);
	  }
	  break;
 
	  case CAPABILITY_QUERY:
	  write(START_SYSEX);
	  write(CAPABILITY_RESPONSE);
	  for (byte pin=0; pin < TOTAL_PINS; pin++) {
		  if (IS_PIN_DIGITAL(pin)) 
		  {
			  write((byte)INPUT);			  
			  write(1);
			  write((byte)OUTPUT);
			  write(1);
		  }
	      if (IS_PIN_PWM(pin)) {
		      write(PWM);
		      write(8);
	      }
		  write(127);
	  }
	  write(END_SYSEX);
	  break;
	
	 case PIN_STATE_QUERY:
	 if (argc > 0) {
		 byte pin=argv[0];
		 write(START_SYSEX);
		 write(PIN_STATE_RESPONSE);
		 write(pin);
		 if (pin < TOTAL_PINS) {
			 write((byte)pinConfig[pin]);
			 write((byte)pinState[pin] & 0x7F);
			 if (pinState[pin] & 0xFF80) write((byte)(pinState[pin] >> 7) & 0x7F);
			 if (pinState[pin] & 0xC000) write((byte)(pinState[pin] >> 14) & 0x7F);
		 }
		 write(END_SYSEX);
	 }
	 break;
	  case UART_COMMAND:  
	  {
		  if(argv[0]==UART_BEGIN)
		  {
			  UartInit(0);
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
	break; 
	case FIRMATA_MUTE:
	{
		if (argv[0]==0)
		{
			muteFlag=0;
		}
		else if (argv[0]==1)
		{
			muteFlag=1;
		}
	}break;

	case IS_ALIVE:
	{
		UartTx1(0xf0);
		UartTx1(IS_ALIVE);
		UartTx1(0xf7);
	}break;
	
	case RESET_MICRO:
	{
		forceHardReset();
	}break;
	
	case PULSE_IN_INIT: 
	{
		
        pinPWM = argv[0];
		
		if(argv[1]==0x00)
		isPulseInEnabled =1;
		else  if(argv[1]==0x04)
		isPulseInEnabled =0;
		
	}break; 
	case RESET_BLUETOOTH:
	{
		if (argv[0])
		{
			resetBluetooth();
		}
		
		responseFlag=true;
	}break;
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
		setPinModeCallback(i, INPUT);
	}
}

// make one instance for the user to use
FirmataClass Firmata ;
