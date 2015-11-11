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

void fillBufferWithPinStates(){
	
	if (port0ChangedFlag)
	{
		int j = 0;
		for (int i = port0Index; i<port0Index+3;i++)
		{
			UartTx1Buffer[i]=digitalPort0array[j];
			j++;
		}
	}
	
	if (port1ChangedFlag)
	{
		int j = 0;
		for (int i = port1Index ; i<port1Index+3;i++)
		{
			UartTx1Buffer[i]=digitalPort1array[j];
			j++;
		}
	}
	
	if (port2ChangedFlag)
	{
		int j = 0;
		for (int i = port2Index ; i<port2Index+3;i++)
		{
			UartTx1Buffer[i]=digitalPort2array[j];
			j++;
		}
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
		processUart0Input();
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
		if ((newMillis-sentFramesMillis)>15)
		{
			if ((muteFlag==0)&&uart1WriteFlag)
			{
				fillBufferWithPinStates();
				writeOnUart1(0xFF);
				for (int i=0; i<getUartTx1BufferCounter(); i++)
				{
					writeOnUart1(UartTx1Buffer[i]);
				}
				
				for (int i=0; i<20; i++)
				{
					UartTx1Buffer[i]=0x00;
				}
				 
				if(firstFrameToSend) firstFrameToSend = false;
				setUartTx1BufferCounter(0);
				uart1WriteFlag=false;
				sentFramesMillis=millis();
				port0ChangedFlag = false;
				port1ChangedFlag = false;
				port2ChangedFlag = false;
				port0Index = 0;
				port1Index = 0;
				port2Index = 0;
				lastFrameSent = true;
			}
		}
	}
}

