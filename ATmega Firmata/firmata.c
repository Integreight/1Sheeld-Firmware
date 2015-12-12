/*

  Project:       1Sheeld Firmware 
  File:          firmata.cpp

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
                 
  Date:          2014.5

*/

#include "firmata.h"
#include <avr/wdt.h>
#include <util/delay.h>

//******************************************************************************
//* Support Functions
//******************************************************************************


void reportDigitalPorts()
{
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

}

void write(unsigned char data)
{
	storeDataInSmallBuffer=true;
	if (txBufferIndex<20)
	{
		UartTx1Buffer[txBufferIndex]=data;
		txBufferIndex++;
	}
}

void sendValueAsTwo7bitBytes(int value)
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
	//isPulseInEnabled =0;
	muteFirmata=0;// 1 for rx1,tx1 with
}

int available(void)
{
	return getAvailableDataCountOnSerial1();
}

void processSysexMessage(void)
{

		sysexCallback(storedInputData[0], sysexBytesRead - 1, storedInputData + 1);
}

void processUart0Input()
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
		
	if(getAvailableDataCountOnUart0()>0)
	{
		int availableBytesInTxBuffer;
		availableBytesInTxBuffer = ((20-txBufferIndex)-3)/2;
		if(getAvailableDataCountOnUart0()<availableBytesInTxBuffer){
			availableBytesInTxBuffer=getAvailableDataCountOnUart0();	
		}
		
		if(!firstFrameToSend){
			byte arr[availableBytesInTxBuffer];
			for(int i=0;i<availableBytesInTxBuffer;i++){
				arr[i]=readFromUart0();
			}
			sendSysex(UART_DATA,availableBytesInTxBuffer,arr);
		}	
	}
	
}

void processInput(void)
{
	int inputData = readFromUart1(); // this is 'int' to handle -1 when no data
	int command;
	
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

void sendDigitalPort(byte portNumber, int portData)
{	
	storeDataInSmallBuffer = true;
	if(portNumber == 0){
			digitalPort0array[0]= DIGITAL_MESSAGE | (portNumber & 0xF);
			digitalPort0array[1]= (byte)portData % 128;
			digitalPort0array[2]= portData >> 7;
			port0StatusChanged =true;
		}else if(portNumber == 1){
			digitalPort1array[0]= DIGITAL_MESSAGE | (portNumber & 0xF);
			digitalPort1array[1]= (byte)portData % 128;
			digitalPort1array[2]= portData >> 7;
			port1StatusChanged =true;
		}else if(portNumber == 2){
			digitalPort2array[0]= DIGITAL_MESSAGE | (portNumber & 0xF);
			digitalPort2array[1]= (byte)portData % 128;
			digitalPort2array[2]= portData >> 7;
			port2StatusChanged=true;
		}
}
			
void sendSysex(byte command, byte bytec, byte* bytev)
{
	byte i;
	startSysex();
	write(command);
	for(i=0; i<bytec; i++) {
		sendValueAsTwo7bitBytes(bytev[i]);
	}
	endSysex();
}

void requestBluetoothReset()
{
	firstFrameToSend = true;
	write(START_SYSEX);
	write(RESET_BLUETOOTH);
	write(END_SYSEX);
}

void sendIsAlive()
{
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
  muteFirmata=0;
  txBufferIndex = 0;
  sysexBytesRead = 0;
  parsingSysex = false;
  storeDataInSmallBuffer=false;
  bluetoothResetResponded=false;
  isAppResponded=false;
  notAliveSentToArduino=false;
  firstFrameToSend = false;
  resendDigitalPort = false;
  resendIsAlive = false ;
  resendPrintVersion = false;
  setIsArduinoRx0BufferEmptyFlag(true) ;
  arduinoStopped =true;
  port0StatusChanged = false;
  port1StatusChanged = false;
  port2StatusChanged = false;
  isPort0StatusEqual = true;
  isPort1StatusEqual = true;
  isPort2StatusEqual = true;
  dataInArduinoBuffer = false;
  toggelingIndicator=false;
  systemResetCallback();
}

void printVersion()
{
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
	
}
// =============================================================================
void sendSysexDataByte(byte command, int value){

	startSysex();
	write(command);
	sendValueAsTwo7bitBytes(value);
	endSysex();

}


/*==============================================================================
 * FUNCTIONS
 *============================================================================*/


void outputPort(byte portNumber, byte portValue, byte forceSend)
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
  //if (reportPINs[3]) outputPort(3, readPort(3, portConfigInputs[3]), false);
  //if (reportPINs[4]) outputPort(4, readPort(4, portConfigInputs[4]), false);
}

// -----------------------------------------------------------------------------
/* sets the pin mode to the correct state and sets the relevant bits in the
 * two bit-arrays that track Digital I/O and PWM status
 */
void setPinModeCallback(byte pin, int mode)
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

void analogWriteCallback(byte pin, int value)
{
	if (pin < TOTAL_PINS) {

			if (IS_PIN_PWM(pin))
			analogWrite(pin, value);
			pinState[pin] = value;

	}
}

void digitalWriteCallback(byte port, int value)
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

void reportDigitalCallback(byte port, int value)
{
	if (port < TOTAL_PORTS) {
		reportPINs[port] = (byte)value;
	}

}

/*==============================================================================
 * SYSEX-BASED commands
 *============================================================================*/

void sysexCallback(byte command, byte argc, byte *argv)
{
  switch(command) {
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
			  initUart(0);
		  }
		  else if(argv[0]==UART_END) terminateUart(0);
		  
	  }break;
	case UART_DATA:
	{
		byte newData [argc/2];
		for (int i = 0; i < argc; i+=2) // run over and over
		{
			newData[i/2]=(argv[i]|(argv[i+1]<<7));
			writeOnUart0(newData[i/2]);
		}
	}
	break; 
	case FIRMATA_MUTE:
	{
		if (argv[0]==0)
		{
			muteFirmata=0;
		}
		else if (argv[0]==1)
		{
			muteFirmata=1;
		}
	}break;

	case IS_ALIVE:
	{
		isAppResponded=true;
		notAliveSentToArduino=false;
		//writeOnUart1(0xf0);
		//writeOnUart1(IS_ALIVE);
		//writeOnUart1(0xf7);
	}break;
	
	case RESET_MICRO:
	{
		forceHardReset();
	}break;
	
	/*case PULSE_IN_INIT: 
	{
		
        pinPWM = argv[0];
		
		if(argv[1]==0x00)
		isPulseInEnabled =1;
		else  if(argv[1]==0x04)
		isPulseInEnabled =0;
		
	}break; */
	case RESET_BLUETOOTH:
	{
		if (argv[0]&&!(argv[1]&argv[2]))
		{
			resetBluetooth();
		}
		
		bluetoothResetResponded=true;
	}break;
	case REPORT_INPUT_PINS:
	{
		reportDigitalPorts();
	}break;
	}
}


void systemResetCallback()
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

void processDataFromApp()
{
	while(available()>0)
	{
		processInput();
	}
}

void checkBluetoothResetResponse()
{
	if (((newMillis-bluetoothResponseMillis)>=RESPONSE_INTERVAL) && (!bluetoothResetResponded) )
	{
		resetBluetooth();
		bluetoothResetResponded = true;
	}
}

void checkAppConnection()
{
	if (!notAliveSentToArduino)
	{
		if((newMillis-isAliveMillis)>=APP_RESPONSE_INTERVAL)
		{
			if (!isAppResponded)
			{
				sendFrameToArduino();
				notAliveSentToArduino = true;
			}
			else
			{
				sendIsAlive();
				isAliveMillis=millis();
				isAppResponded = false;
			}
		}
	}
}

void sendDataToApp()
{
	if ((newMillis-sentFramesMillis)> FRAME_GAP && (muteFirmata==0) && storeDataInSmallBuffer)
	{
			if (dataInArduinoBuffer)
			{
				if (!toggelingIndicator)
				{
					toggelingIndicator=true;
				}
				else
				{
					if(port0StatusChanged)fillBufferWithPinStates(digitalPort0array,0);
					if(port1StatusChanged)fillBufferWithPinStates(digitalPort1array,1);
					if(port2StatusChanged)fillBufferWithPinStates(digitalPort2array,2);
					toggelingIndicator= false;
				}
				
			}else{
				if(port0StatusChanged)fillBufferWithPinStates(digitalPort0array,0);
				if(port1StatusChanged)fillBufferWithPinStates(digitalPort1array,1);
				if(port2StatusChanged)fillBufferWithPinStates(digitalPort2array,2);
			}
			processUart0Input();
			for (int i=0; i<txBufferIndex; i++)
			{
				writeOnUart1(UartTx1Buffer[i]);
			}
			if(firstFrameToSend) firstFrameToSend = false;
			txBufferIndex = 0;
			storeDataInSmallBuffer=false;
			port0StatusChanged = false;
			port1StatusChanged = false;
			port2StatusChanged = false;
			sentFramesMillis=millis();
	}
}

int checkPortStateEquality(byte * oldPort ,byte * newPort,byte numberOfPins)
{
	while(--numberOfPins>0 && oldPort[numberOfPins]==newPort[numberOfPins]);
	return numberOfPins!=0;
}

void fillBufferWithPinStates(byte * portArray,byte portNumber)
{
	if(portNumber == 0)
	{
		if(checkPortStateEquality(oldDigitalPort0array,portArray,3)){
			isPort0StatusEqual = false;
			for(int i = 0 ;i <3 ; i++) oldDigitalPort0array[i]=portArray[i];
			}else{
			isPort0StatusEqual = true;
		}
		}else if(portNumber == 1){
		if(checkPortStateEquality(oldDigitalPort1array,portArray,3)){
			isPort1StatusEqual = false;
			for(int i = 0 ;i <3 ; i++) oldDigitalPort1array[i]=portArray[i];
			}else{
			isPort1StatusEqual = true;
		}
		}else if(portNumber == 2){
		if(checkPortStateEquality(oldDigitalPort2array,portArray,3)){
			isPort2StatusEqual = false;
			for(int i = 0 ;i <3 ; i++) oldDigitalPort2array[i]=portArray[i];
			}else{
			isPort2StatusEqual = true;
		}
	}
	if (txBufferIndex+3<20 && ((!isPort0StatusEqual)||(!isPort1StatusEqual)||(!isPort2StatusEqual))){
		int j = 0;
		for (int i = txBufferIndex; i<txBufferIndex+3 ;i++)
		{
			UartTx1Buffer[i]=portArray[j];
			j++;
		}
		txBufferIndex+=3;
		isPort0StatusEqual = true;
		isPort1StatusEqual = true;
		isPort2StatusEqual = true;
	}
}