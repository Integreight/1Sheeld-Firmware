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
#ifdef PLUS_BOARD
void checkIfPinsChangedSendThem()
{
	fillBufferWithPinStates(digitalPort0array,0);
	fillBufferWithPinStates(digitalPort1array,1);
	fillBufferWithPinStates(digitalPort2array,2);
}
#endif // PLUS_BOARD


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
			checkNeededDataToResend();		
			processUart0Input();	
		}
		else
		{
			checkIfPinsChangedSendThem();
			checkNeededDataToResend();
		}

		if(txBufferIndex!=0)
		{
			for (uint16_t i=0; i<txBufferIndex; i++)
			{
				writeOnUart1(UartTx1Buffer[i]);
			}
			txBufferIndex = 0;
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
	uint8_t dataArray[7]={0xFF,0x00,0xF0,0x04,0x00,0xFF,0x00};
	for (uint16_t i = 0; i < 7; i++)
	{
		writeOnUart0(dataArray[i]);
	}
}

void sendArduinoToSendData()
{
	uint8_t dataArray[7]={0xFF,0x00,0xF0,0x05,0x00,0xFF,0x00};
	for (uint16_t i = 0; i < 7; i++)
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

void checkPortStateEquality(uint8_t * oldPort ,uint8_t * newPort,uint8_t portNumber, uint8_t numberOfBytesToCheck)
{
	uint8_t count=0;
	while(--numberOfBytesToCheck>=0 && oldPort[numberOfBytesToCheck]==newPort[numberOfBytesToCheck]) count++;
	if(count!=2){
		for(uint16_t i = 0 ;i <2 ; i++) oldPort[i]=newPort[i];
		if (txBufferIndex+3 < 20){
			write(DIGITAL_MESSAGE | (portNumber & 0xF));
			for (uint16_t i = 0; i< 2 ;i++)write(newPort[i]);
		}
	}
}

void fillBufferWithPinStates(uint8_t * portArray,uint8_t portNumber)
{
	switch (portNumber){
		case 0: 
			checkPortStateEquality(oldDigitalPort0array,portArray,portNumber,2);
			break;
		
		case 1: 
			checkPortStateEquality(oldDigitalPort1array,portArray,portNumber,2);
			break;
		
		case 2:
			checkPortStateEquality(oldDigitalPort2array,portArray,portNumber,2);
			break;
		
		default:
			break;
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