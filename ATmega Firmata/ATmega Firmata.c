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

