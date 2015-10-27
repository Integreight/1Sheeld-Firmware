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
	byte dataArray[7]={0xff,0x00,0x00,0x02,0x00,0xff,0x00};
	for (int i = 0; i < 7; i++)
	{
		writeOnUart0(dataArray[i]);
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
		processUart0Input();
		checkDigitalInputs();
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
		if ((millis()-sentFramesMillis)>15)
		{
			if ((muteFlag==0)&&uart1WriteFlag)
			{
				for (int i=0; i<=txBufferIndex; i++)
				{
					writeOnUart1(UartTx1Buffer[i]);
				} 
				writeOnUart1('e');
				uart1WriteFlag=false;
				sentFramesMillis=millis();
			}
		}
		if (getAvailableDataCountOnSerial0()<192)
		{
			writeOnUart0('s');
		}
		else
		{
			writeOnUart0('t');
		}
	}
}

