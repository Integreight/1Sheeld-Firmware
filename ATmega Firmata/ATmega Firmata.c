/*

  Project:       1Sheeld Firmware 
  File:          ATmega Firmata.cpp

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
                 
  Date:          2014.5

*/

#include "1sheelds_functions.h"
#include "firmata.h"
#include "timers.h"

unsigned long currentMillis;
unsigned long newMillis;
unsigned long responseInterval =200UL ;
unsigned long frameGap = 15UL ;
unsigned long isAliveMillis;
void setupMillisTimers()
{
	TCCR0=(1<<CS00)|(1<<CS01);
	SET_BIT(TIMSK,TOIE0);
}

void setupUartLeds()
{
	SET_BIT(DDRA,6);
	SET_BIT(DDRA,7);
	SET_BIT(PORTA,6);
	SET_BIT(PORTA,7);
	TCCR2|=(1<<CS20)|(1<<CS21); // clock prescalar =32
}

void sendFrameToArduino()
{
	byte dataArray[7]={0xff,0x00,0xF0,0x02,0x00,0xff,0x00};
	for (int i = 0; i < 7; i++)
	{
		writeOnUart0(dataArray[i]);
	}
}

void sendArduinoToStopData()
{
	byte dataArray[3]={0xf0,0x70,0xf7};
	for (int i = 0; i < 3; i++)
	{
		writeOnUart0(dataArray[i]);
	}
}

void sendArduinoToSendData()
{
	byte dataArray[3]={0xf0,0x71,0xf7};
	for (int i = 0; i < 3; i++)
	{
		writeOnUart0(dataArray[i]);
	}
}

int checkPortStateEquality(byte * oldPort ,byte * newPort,byte numberOfPins)
{
	while(--numberOfPins>0 && oldPort[numberOfPins]==newPort[numberOfPins]);
	return numberOfPins!=0;
}

void fillBufferWithPinStates(byte * portArray,byte portNumber){
	if(portNumber == 0){
		if(checkPortStateEquality(oldDigitalPort0array,portArray,3)){
			 port0StateEqual = false;
			 for(int i = 0 ;i <3 ; i++) oldDigitalPort0array[i]=portArray[i];
		}else{
			port0StateEqual = true;
		}
	}else if(portNumber == 1){
		if(checkPortStateEquality(oldDigitalPort1array,portArray,3)){
			port1StateEqual = false;
			for(int i = 0 ;i <3 ; i++) oldDigitalPort1array[i]=portArray[i];
			}else{
			port1StateEqual = true;
		}
	}else if(portNumber == 2){
		if(checkPortStateEquality(oldDigitalPort2array,portArray,3)){
			port2StateEqual = false;
			for(int i = 0 ;i <3 ; i++) oldDigitalPort2array[i]=portArray[i];
			}else{
			port2StateEqual = true;
		}
	}
	if (txBufferIndex+3<20 && ((!port0StateEqual)||(!port1StateEqual)||(!port2StateEqual))){
		int j = 0;
		for (int i = txBufferIndex; i<txBufferIndex+3 ;i++)
		{
			UartTx1Buffer[i]=portArray[j];
			j++;
		}
		txBufferIndex+=3;
		port0StateEqual = true;
		port1StateEqual = true;
		port2StateEqual = true;
	}	
}

int main(void)
{ 
	setupMillisTimers();
	sei();
	initFirmata();
	setUnusedPinsAsOutput();
	requestBluetoothReset();
	currentMillis=millis();
	isAliveMillis=millis();
	sentFramesMillis=millis();
	setupUartLeds();
    initUart(0);
	sendIsAlive();
	while (1)
	{		
		checkDigitalInputs();
		if (getAvailableDataCountOnUart0()>0){
			dataInArduinoBuffer = true;
			uart1WriteFlag = true;
		}
		else{
			dataInArduinoBuffer = false;
		}
		
		while(available()>0)
        {
           processInput();
        }
		newMillis = millis();
		if (((newMillis-currentMillis)>=responseInterval) && (!getBtResponseFlag()) )
		{
		   resetBluetooth();
           setBtResponseFlag(true);
		}
		
		if (sendArduinoToStopFlag)
		{
			sendArduinoToStopData();
			sendArduinoToStopFlag = false;
			arduinoStopped = true;
		}
		
		if(sendArduinoToSendFlag){
			sendArduinoToSendData();
			sendArduinoToSendFlag = false;
			arduinoStopped = false;
		}
		
		if (!getIsAliveFrameNotSent())
		{
			if((newMillis-isAliveMillis)>=500)
			{
				if ((!getIsAliveResponseFlag()))
				{
					sendFrameToArduino();
					setIsAliveFrameNotSent(true);
				}
				else
				{
					sendIsAlive();
					isAliveMillis=millis();
					setIsAliveResponseFlag(false);
				}
			}
		}
		if ((newMillis-sentFramesMillis)> frameGap)
		{
			if ((muteFlag==0)&&uart1WriteFlag)
			{
				if (dataInArduinoBuffer)
				{
					if (!toggelingIndicator)
					{
						toggelingIndicator=true;
					}
					else
					{
						if(port0ChangedFlag)fillBufferWithPinStates(digitalPort0array,0);
						if(port1ChangedFlag)fillBufferWithPinStates(digitalPort1array,1);
						if(port2ChangedFlag)fillBufferWithPinStates(digitalPort2array,2);
						toggelingIndicator= false;
					}	
				}else{
						if(port0ChangedFlag)fillBufferWithPinStates(digitalPort0array,0);
						if(port1ChangedFlag)fillBufferWithPinStates(digitalPort1array,1);
						if(port2ChangedFlag)fillBufferWithPinStates(digitalPort2array,2);	
				}
				processUart0Input();
				writeOnUart1(0xFF);
				for (int i=0; i<getUartTx1BufferCounter(); i++)
				{
					writeOnUart1(UartTx1Buffer[i]);
				}
				if(firstFrameToSend) firstFrameToSend = false;
				setUartTx1BufferCounter(0);	
				uart1WriteFlag=false;
				port0ChangedFlag = false;
				port1ChangedFlag = false;
				port2ChangedFlag = false;
				sentFramesMillis=millis();
			}
		}
	}
}

