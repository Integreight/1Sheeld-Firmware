/*

  Project:       1Sheeld Firmware 
  File:          onesheeld.cpp

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
                 
  Date:          2014.5

*/

#include "config.h"
#include "onesheeld.h"

void sendArduinoAppDisconnected()
{
	uint8_t dataArray[7]={0xff,0x00,0xF0,0x02,0x00,0xff,0x00};
	for (uint16_t i = 0; i < 7; i++)
	{
		writeOnUart0(dataArray[i]);
	}
}

void initialization()
{
	sei();
	setupMillisTimers();
	initFirmata();
	systemReset();
	initUart(1,BAUD_115200);
	#ifdef PLUS_BOARD
	initUart(0,getSavedBaudRateFromEeprom());
	#endif
	#ifdef CLASSIC_BOARD
	initUart(0,BAUD_115200);
	#endif
	setUnusedPinsAsOutput();
	setupUartLeds();
	requestBluetoothReset();
	sendIsAlive();
}

void catchTimeForSomeVariables()
{
	bluetoothResponseMillis=millis();
	isAliveMillis=millis();
	#ifdef PLUS_BOARD
	sentFramesMillis=millis();
	#endif // PLUS_BOARD
}

void checkDigitalPinStatus()
{
	checkDigitalInputs();
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
	if (((newMillis-bluetoothResponseMillis)>=BLUETOOH_RESET_RESPONSE_INTERVAL) && (!bluetoothResetResponded) )
	{
		resetBluetooth();
		bluetoothResetResponded = true;
	}
}

void checkAppConnection()
{
	if (!notAliveSentToArduino)
	{
		if((newMillis-isAliveMillis)>=APP_ISALIVE_RESPONSE_INTERVAL)
		{
			if (!isAppResponded)
			{
				sendArduinoAppDisconnected();
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

void checkIfPinsChangedSendThem()
{
	if(port0StatusChanged)fillBufferWithPinStates(digitalPort0array,0);
	if(port1StatusChanged)fillBufferWithPinStates(digitalPort1array,1);
	if(port2StatusChanged)fillBufferWithPinStates(digitalPort2array,2);
}

void sendDataToApp()
{
	#ifdef PLUS_BOARD
	if ((newMillis-sentFramesMillis)> FRAME_GAP && (muteFirmata==0))
	{
		if (getAvailableDataCountOnUart0()>0)
		{
			if (!toggelingIndicator)
			{
				toggelingIndicator=true;
			}
			else
			{
				checkIfPinsChangedSendThem();
				toggelingIndicator= false;
			}	
		}
		else
		{
			checkIfPinsChangedSendThem();
		}
		
		processUart0Input();
		if(txBufferIndex!=0)
		{
			for (uint16_t i=0; i<txBufferIndex; i++)
			{
				writeOnUart1(UartTx1Buffer[i]);
			}
			if(firstFrameToSend) firstFrameToSend = false;
			txBufferIndex = 0;
			port0StatusChanged = false;
			port1StatusChanged = false;
			port2StatusChanged = false;
		}
		sentFramesMillis=millis();
	}
	#endif
	#ifdef CLASSIC_BOARD
	processUart0Input();
	#endif
	
}

#ifdef PLUS_BOARD
void sendArduinoToStopData()
{
	uint8_t dataArray[10]={0xFF,0x00,0xF0,0x04,0x01,0xFE,0x01,0xFE,0x01,0x00};
	for (uint16_t i = 0; i < 10; i++)
	{
		writeOnUart0(dataArray[i]);
	}
}

void sendArduinoToSendData()
{
	uint8_t dataArray[10]={0xFF,0x00,0xF0,0x04,0x01,0xFE,0x01,0xFE,0x00,0x00};
	for (uint16_t i = 0; i < 10; i++)
	{
		writeOnUart0(dataArray[i]);
	}
}

void checkArduinoRx0BufferSpace()
{
	if(getIsArduinoRx0BufferOverFlowedFlag())
	{
		sendArduinoToStopData();
		setIsArduinoRx0BufferOverFlowedFlag(false);
		arduinoStopped=true;
	}
	else if (!getIsArduinoRx0BufferEmptyFlag() && !arduinoStopped)
	{
		sendArduinoToStopData();
		arduinoStopped = true;
	}
	else if(getIsArduinoRx0BufferEmptyFlag() && arduinoStopped){
		sendArduinoToSendData();
		arduinoStopped = false;
	}
}

uint16_t checkPortStateEquality(uint8_t * oldPort ,uint8_t * newPort,uint8_t numberOfPins)
{
	while(--numberOfPins>0 && oldPort[numberOfPins]==newPort[numberOfPins]);
	return numberOfPins!=0;
}

void fillBufferWithPinStates(uint8_t * portArray,uint8_t portNumber)
{
	if(portNumber == 0)
	{
		if(checkPortStateEquality(oldDigitalPort0array,portArray,3)){
			isPort0StatusEqual = false;
			for(uint16_t i = 0 ;i <3 ; i++) oldDigitalPort0array[i]=portArray[i];
			}else{
			isPort0StatusEqual = true;
		}
		}else if(portNumber == 1){
		if(checkPortStateEquality(oldDigitalPort1array,portArray,3)){
			isPort1StatusEqual = false;
			for(uint16_t i = 0 ;i <3 ; i++) oldDigitalPort1array[i]=portArray[i];
			}else{
			isPort1StatusEqual = true;
		}
		}else if(portNumber == 2){
		if(checkPortStateEquality(oldDigitalPort2array,portArray,3)){
			isPort2StatusEqual = false;
			for(uint16_t i = 0 ;i <3 ; i++) oldDigitalPort2array[i]=portArray[i];
			}else{
			isPort2StatusEqual = true;
		}
	}
	uint8_t bufferIndex = txBufferIndex;
	if (bufferIndex+3<20 && ((!isPort0StatusEqual)||(!isPort1StatusEqual)||(!isPort2StatusEqual))){
		uint16_t j = 0;
		for (uint16_t i = bufferIndex; i<bufferIndex+3 ;i++)
		{
			write(portArray[j]);
			j++;
		}
		isPort0StatusEqual = true;
		isPort1StatusEqual = true;
		isPort2StatusEqual = true;
	}
}

uint8_t getSavedBaudRateFromEeprom()
{
	uint8_t savedBaudRate = readFromEeprom(CURRENT_UART0_BAUD_RATE_EEPROM_ADDRESS);
	switch (savedBaudRate)
	{
		case BAUD_9600:
		case BAUD_14400:
		case BAUD_19200:
		case BAUD_28800:
		case BAUD_38400:
		case BAUD_57600:
		case BAUD_115200:	break;
		default:
		{
			savedBaudRate = BAUD_115200;
			updateEeprom(CURRENT_UART0_BAUD_RATE_EEPROM_ADDRESS,savedBaudRate);
		}
	}
	return savedBaudRate;
}
#endif // PLUS_BOARD