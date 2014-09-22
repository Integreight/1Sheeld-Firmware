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
int main(void)
{ 
	setupMillisTimers();
	sei();
	initFirmata();
	systemResetCallback();
	setUnusedPinsAsOutput();
	requestBluetoothReset();
	currentMillis=millis();
	setupUartLeds();
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
	}
}

