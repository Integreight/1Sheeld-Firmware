/*

  Project:       1Sheeld Firmware 
  File:          firmata.cpp

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
                 
  Date:          2014.5

*/

#include "config.h"
#include "firmata.h"
#include <avr/wdt.h>
#include <util/delay.h>

//******************************************************************************
//* Support Functions
//******************************************************************************

const uint8_t atNameArray[7] PROGMEM = {'A','T','+','N','A','M','E'};
	
void reportDigitalPorts()
{
	#ifdef PLUS_BOARD
	if((!firstFrameToSend) && txBufferIndex +10 >20)
	{
		resendDigitalPort = true;
	}else
	{
		resendDigitalPort =false;
		outputPort(0, readPort(0, portConfigInputs[0]), true);
		outputPort(1, readPort(1, portConfigInputs[1]), true);
		outputPort(2, readPort(2, portConfigInputs[2]), true);
	}
	#endif
	#ifdef CLASSIC_BOARD
	outputPort(0, readPort(0, portConfigInputs[0]), true);
	outputPort(1, readPort(1, portConfigInputs[1]), true);
	outputPort(2, readPort(2, portConfigInputs[2]), true);
	#endif
}

void write(uint8_t data)
{
	#ifdef PLUS_BOARD
	if (txBufferIndex<20)
	{
		UartTx1Buffer[txBufferIndex]=data;
		txBufferIndex++;
	}
	#endif
	#ifdef CLASSIC_BOARD
	if (muteFirmata == 0)
	{
		writeOnUart1(data);
	}
	#endif
}

void sendValueAsTwo7bitBytes(int16_t value)
{
  write(value & 0b01111111); // LSB
  write(value >> 7 & 0b01111111); // MSB
}

void startSysex(void)
{
  write(START_SYSEX);
}

void endSysex(void)
{
  write(END_SYSEX);
}


//******************************************************************************
//* Public Methods
//******************************************************************************

void forceHardReset()
{
	cli();
	// disable interrupts
	wdt_enable(WDTO_15MS);
	// enable watchdog
	while(1);
	// wait for watchdog to reset processor
}

/* begin method for overriding default serial bitrate */
void initFirmata()
{
	muteFirmata=0;// 1 for rx1,tx1 with
}

int16_t available(void)
{
	return getAvailableDataCountOnUart1();
}

void processSysexMessage(void)
{

		sysexCallback(storedInputData[0], sysexBytesRead - 1, storedInputData + 1);
}

#ifdef PLUS_BOARD
void checkNeededDataToResend()
{
	if (resendIsAlive)
	{
		sendIsAlive();
	}
	
	if(resendDigitalPort)
	{
		reportDigitalPorts();
	}
	
	if (resendPrintVersion)
	{
		printVersion();
	}
	
	if (resendTestingAnswer)
	{
		sendAnswerToApplication();
	}
	
	if (resendCurrentBaudRate)
	{
		getCurrentUart0BaudRate();
	}
}
#endif

void processUart0Input()
{
	#ifdef PLUS_BOARD
	if(getAvailableDataCountOnUart0()>0)
	{
		if (txBufferIndex <=15)
		{
			uint8_t availableBytesInTxBuffer;
			availableBytesInTxBuffer = ((20-txBufferIndex)-3)/2;
			uint8_t countOfDataInUart0Buffer = getAvailableDataCountOnUart0();
			if(countOfDataInUart0Buffer < availableBytesInTxBuffer)
			{
				availableBytesInTxBuffer= countOfDataInUart0Buffer;
			}
			
			if(!firstFrameToSend)
			{
				uint8_t arr[availableBytesInTxBuffer];
				for(uint16_t i=0;i<availableBytesInTxBuffer;i++){
					arr[i]=readFromUart0();
				}
				sendSysex(UART_DATA,availableBytesInTxBuffer,arr);
			}
		}
	}
	#endif
	#ifdef CLASSIC_BOARD
		uint16_t availableData=getAvailableDataCountOnUart0();
		if(availableData>0){
			uint8_t arr[availableData];
			for(uint16_t i=0;i<availableData;i++){
				arr[i]=readFromUart0();
			}
			sendSysex(UART_DATA,availableData,arr);
		}
	#endif
}

void processInput(void)
{
	int16_t inputData = readFromUart1(); // this is 'int' to handle -1 when no data
	int16_t command;
	
	// TODO make sure it handles -1 properly

	if (parsingSysex) {
		if(inputData == END_SYSEX) {
			//stop sysex byte
			parsingSysex = false;
			//fire off handler function
			processSysexMessage();
			} else if(sysexBytesRead>=MAX_DATA_BYTES) {
				parsingSysex = false;
			}
			else{
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

void sendDigitalPort(uint8_t portNumber, int16_t portData)
{
	#ifdef PLUS_BOARD
	switch(portNumber){
		
		case 0:
				digitalPort0array[0]= (uint8_t)portData % 128;
				digitalPort0array[1]= portData >> 7;
				break;
		case 1:
				digitalPort1array[0]= (uint8_t)portData % 128;
				digitalPort1array[1]= portData >> 7;
				break;
		case 2:
				digitalPort2array[0]= (uint8_t)portData % 128;
				digitalPort2array[1]= portData >> 7;
				break;
		default:
		break;
	}
	#endif
	#ifdef CLASSIC_BOARD
	write(DIGITAL_MESSAGE | (portNumber & 0xF));
	write((uint8_t)portData % 128); // Tx bits 0-6
	write(portData >> 7);  // Tx bits 7-13
	#endif
	
}
			
void sendSysex(uint8_t command, uint8_t bytec, uint8_t* bytev)
{
	uint8_t i;
	startSysex();
	write(command);
	for(i=0; i<bytec; i++) {
		sendValueAsTwo7bitBytes(bytev[i]);
	}
	endSysex();
}

void requestBluetoothReset()
{
	#ifdef PLUS_BOARD
	firstFrameToSend = true;
	#endif
	write(START_SYSEX);
	write(RESET_BLUETOOTH);
	write(END_SYSEX);
}

void sendIsAlive()
{
	#ifdef PLUS_BOARD
	if((!firstFrameToSend) && (txBufferIndex +3 >20))
	{
		resendIsAlive = true;
	}
	else
	{
		write(START_SYSEX);
		write(IS_ALIVE);
		write(END_SYSEX);
		resendIsAlive = false;
	}
	#endif
	#ifdef CLASSIC_BOARD
	write(START_SYSEX);
	write(IS_ALIVE);
	write(END_SYSEX);
	#endif
	
}

//******************************************************************************
//* Private Methods
//******************************************************************************
// resets the system state upon a SYSTEM_RESET message from the host software
void systemReset(void)
{
  waitForData = 0; // this flag says the next serial input will be data
  executeMultiByteCommand = 0; // execute this after getting multi-byte data
  multiByteChannel = 0; // channel data for multiByteCommands
  parsingSysex = false;
  sysexBytesRead = 0;
  bluetoothResetResponded=false;
  isAppResponded=false;
  notAliveSentToArduino=false;
  systemResetCallback();
  #ifdef PLUS_BOARD
  txBufferIndex = 0;
  firstFrameToSend = false;
  resendDigitalPort = false;
  resendIsAlive = false ;
  resendPrintVersion = false;
  resendCurrentBaudRate = false;
  resendTestingAnswer = false;
  setIsArduinoRx0BufferEmptyFlag(true) ;
  setIsArduinoRx0BufferOverFlowedFlag(false);
  arduinoStopped =false;
  toggelingIndicator=false;
  #endif
}

void printVersion()
{
	#ifdef PLUS_BOARD
	if ((!firstFrameToSend) && (txBufferIndex + 3 >20))
	{
		resendPrintVersion = true;
	}
	else
	{
		write(REPORT_VERSION);
		write(ONESHEELD_MINOR_FIRMWARE_VERSION);
		write(ONESHEELD_MAJOR_FIRMWARE_VERSION);
		resendPrintVersion = false;
	}
	#endif
	#ifdef CLASSIC_BOARD
	write(REPORT_VERSION);
	write(ONESHEELD_MINOR_FIRMWARE_VERSION);
	write(ONESHEELD_MAJOR_FIRMWARE_VERSION);
	#endif	
}

/*==============================================================================
 * FUNCTIONS
 *============================================================================*/


void outputPort(uint8_t portNumber, uint8_t portValue, uint8_t forceSend)
{
  // pins not configured as INPUT are cleared to zeros
  portValue = portValue & portConfigInputs[portNumber];
  // only send if the value is different than previously sent
  if(forceSend || previousPINs[portNumber] != portValue) {
    sendDigitalPort(portNumber, portValue);
    previousPINs[portNumber] = portValue;
  }
  
}

/* -----------------------------------------------------------------------------
 * check all the active digital inputs for change of state, then add any events
 * to the Serial output queue using Serial.print() */
void checkDigitalInputs(void)
{
  /* Using non-looping code allows constants to be given to readPort().
   * The compiler will apply substantial optimizations if the inputs
   * to readPort() are compile-time constants. */
  if (reportPINs[0]) outputPort(0, readPort(0, portConfigInputs[0]), false);
  if (reportPINs[1]) outputPort(1, readPort(1, portConfigInputs[1]), false);
  if (reportPINs[2]) outputPort(2, readPort(2, portConfigInputs[2]), false);
}

// -----------------------------------------------------------------------------
/* sets the pin mode to the correct state and sets the relevant bits in the
 * two bit-arrays that track Digital I/O and PWM status
 */
void setPinModeCallback(uint8_t pin, int16_t mode)
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
      setPinMode(pin, INPUT); // disable output driver
      digitalWrite(pin, LOW); // disable internal pull-ups
      pinConfig[pin] = INPUT;
    }
    break;
  case OUTPUT:
    if (IS_PIN_DIGITAL(pin)) {
      digitalWrite(pin, LOW); // disable PWM
      setPinMode(pin, OUTPUT);
      pinConfig[pin] = OUTPUT;

    }
    break;
  case PWM:
    if (IS_PIN_PWM(pin)) {
      setPinMode(pin, OUTPUT);
      analogWrite(pin, 0);
      pinConfig[pin] = PWM;
    }
    break;
  default:
    break;
  }
}

void analogWriteCallback(uint8_t pin, int16_t value)
{
	if (pin < TOTAL_PINS) {

			if (IS_PIN_PWM(pin))
			analogWrite(pin, value);
			pinState[pin] = value;

	}
}

void digitalWriteCallback(uint8_t port, int16_t value)
{
	uint8_t pin, lastPin, mask=1, pinWriteMask=0;

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
					pinState[pin] = ((uint8_t)value & mask) ? 1 : 0;
				}
			}
			mask = mask << 1;
		}
		writePort(port, (uint8_t)value, pinWriteMask);
	}
}

void reportDigitalCallback(uint8_t port, int16_t value)
{
	if (port < TOTAL_PORTS) {
		reportPINs[port] = (uint8_t)value;
	}

}

/*==============================================================================
 * SYSEX-BASED commands
 *============================================================================*/

void sysexCallback(uint8_t command, uint8_t argc, uint8_t *argv)
{
  switch(command) {
	case UART_DATA:
	{
		uint8_t newData [argc/2];
		for (uint16_t i = 0; i < argc; i+=2) // run over and over
		{
			newData[i/2]=(argv[i]|(argv[i+1]<<7));
			writeOnUart0(newData[i/2]);
		}
	}
	break; 
	case FIRMATA_MUTE:
	{
		if ((uint8_t)(argv[0]|(argv[1]<<7))==0)
		{
			muteFirmata=0;
		}
		else if ((uint8_t)(argv[0]|(argv[1]<<7))==1)
		{
			muteFirmata=1;
		}
	}break;

	case IS_ALIVE:
	{
		isAppResponded=true;
		notAliveSentToArduino=false;
	}break;
	
	case RESET_MICRO:
	{
		forceHardReset();
	}break;

	case RESET_BLUETOOTH:
	{
		if ((uint8_t)(argv[0]|argv[1]<<7) &&!(((uint8_t)(argv[2]|argv[3]<<7))&((uint8_t)(argv[4]|argv[5]<<7))))
		{
			resetBluetooth();
		}
		
		bluetoothResetResponded=true;
	}break;
	
	case REPORT_INPUT_PINS:
	{
		reportDigitalPorts();
	}break;
	
	case BLUETOOTH_RENAMING:
	{
		if (argc > 0)
		{
			_delay_ms(100);
			sendBluetoothRenameConfirmation();
			_delay_ms(100);
			resetBluetooth();					/* reset bluetooth to enter AT mode. */
			_delay_ms(1000);					
			sendATNameCommand();				/* send the AT+NAME command to rename the bluetooth. */
			uint8_t newName [argc/2];			/* newName array to re-assemble name from sysex message*/
			for (uint16_t i = 0; i < argc; i+=2) 
			{
				newName[i/2]=(argv[i]|(argv[i+1]<<7));
				writeOnUart1(newName[i/2]);
			} /* send New name*/
			_delay_ms(1000);					/* 1 Second was chosen to support latecny of HC-06 bluetooth module */ 
			resetBluetooth();					/* reset the bluetooth again to restart with new configuration. */
		}		
	}break;
	
	case TESTING_FRAME:
	{
		if (argc>0)
		{
			uint8_t testNumbers [argc/2];			/* newName array to re-assemble name from sysex message*/
			uint16_t sumOfData = 0;
			for (uint16_t i = 0; i < argc; i+=2) testNumbers[i/2]=(argv[i]|(argv[i+1]<<7));
			for (uint16_t i =0 ; i< argc/2 ;i++) sumOfData+=testNumbers[i];
			testAnswer = sumOfData%256;
			sendAnswerToApplication();
		}
	}break;
	#ifdef PLUS_BOARD
		case SET_UART0_BAUD_RATE:
		{
			if (argc == 6)
			{
				uint8_t data [argc/2];			
				for (uint16_t i = 0; i < argc; i+=2) data[i/2]=(argv[i]|(argv[i+1]<<7));
				if(!(data[1] & data[2]))
				{
					//update the new value in eeprom and initialize the uart with the new value
					uint8_t newBaudRateValue = data[0];
					updateEeprom(CURRENT_UART0_BAUD_RATE_EEPROM_ADDRESS,newBaudRateValue);
					initUart(0,newBaudRateValue);
				}
			}
		}break;
		
		case QUERY_UART0_BAUD_RATE:
		{
			getCurrentUart0BaudRate();
		}break;
	#endif // PLUS_BOARD
	}
}

#ifdef PLUS_BOARD
void getCurrentUart0BaudRate()
{
	uint8_t baudRate = readFromEeprom(CURRENT_UART0_BAUD_RATE_EEPROM_ADDRESS);
	if(txBufferIndex + 5 >20)
	{
		resendCurrentBaudRate = true;
	}
	else
	{
		sendSysex(QUERY_UART0_BAUD_RATE,1,&baudRate);
		resendCurrentBaudRate = false;
	}
}
#endif

void sendAnswerToApplication()
{
	#ifdef PLUS_BOARD
	if(txBufferIndex + 5 >20)
	{
		resendTestingAnswer = true;
	}
	else
	{
		sendSysex(TESTING_FRAME,1,&testAnswer);
		resendTestingAnswer = false;
	}
	#endif
	#ifdef CLASSIC_BOARD
	sendSysex(TESTING_FRAME,1,&testAnswer);
	#endif
}

void sendBluetoothRenameConfirmation()
{
	writeOnUart1(START_SYSEX);
	writeOnUart1(BLUETOOTH_RENAMING);
	writeOnUart1(END_SYSEX);
}

void sendATNameCommand()
{
	for(uint16_t i = 0 ; i < 7 ; i++ )writeOnUart1(pgm_read_byte(&(atNameArray[i])));
}

void resetBluetooth()
{
	//bt reset
	SET_BIT(DDRE,0);
	SET_BIT(PORTE,0);
	_delay_ms(5);
	CLR_BIT(PORTE,0);
}

void systemResetCallback()
{	
	for (uint8_t i=0; i < TOTAL_PORTS; i++) {
		reportPINs[i] = false;      // by default, reporting off
		portConfigInputs[i] = 0;	// until activated
		previousPINs[i] = 0;
	}
	
	for (uint8_t i=0; i < TOTAL_PINS; i++)
    {
		setPinModeCallback(i, INPUT);
	}
}
