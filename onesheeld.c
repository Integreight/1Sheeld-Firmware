/*

  Project:       1Sheeld Firmware 
  File:          onesheeld.cpp

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
                 
  Date:          2014.5

*/

#include "config.h"
#include "onesheeld.h"

void sendFrameToArduino()
{
	byte dataArray[7]={0xff,0x00,0xF0,0x02,0x00,0xff,0x00};
	for (int i = 0; i < 7; i++)
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
	initUart(1);
	initUart(0);
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
	#ifdef PLUS_BOARD
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
	#elif defined(CLASSIC_BOARD)
	processUart0Input();
	#endif
	
}

#ifdef PLUS_BOARD
void sendArduinoToStopData()
{
	byte dataArray[10]={0xFF,0x00,0xF0,0x04,0x01,0xFE,0x01,0xFE,0x01,0x00};
	for (int i = 0; i < 10; i++)
	{
		writeOnUart0(dataArray[i]);
	}
}

void sendArduinoToSendData()
{
	byte dataArray[10]={0xFF,0x00,0xF0,0x04,0x01,0xFE,0x01,0xFE,0x00,0x00};
	for (int i = 0; i < 10; i++)
	{
		writeOnUart0(dataArray[i]);
	}
}

void checkDataAvailabilityInRx0Buffer()
{
	if (getAvailableDataCountOnUart0()>0){
		dataInArduinoBuffer = true;
		storeDataInSmallBuffer = true;
	}
	else{
		dataInArduinoBuffer = false;
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
	uint8 bufferIndex = txBufferIndex;
	if (bufferIndex+3<20 && ((!isPort0StatusEqual)||(!isPort1StatusEqual)||(!isPort2StatusEqual))){
		int j = 0;
		for (int i = bufferIndex; i<bufferIndex+3 ;i++)
		{
			write(portArray[j]);
			j++;
		}
		isPort0StatusEqual = true;
		isPort1StatusEqual = true;
		isPort2StatusEqual = true;
	}
}
#endif // PLUS_BOARD