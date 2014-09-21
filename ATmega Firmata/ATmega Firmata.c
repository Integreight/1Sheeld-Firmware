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

unsigned long currentMillis;        // store the current value from millis()
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
	// for millis fn 
	setupMillisTimers();
	sei(); // enable global interrupt
	initFirmata();
	systemResetCallback();  // reset to default config
	setUnusedPinsAsOutput();
	requestBluetoothReset();
	currentMillis=millis();
	//make 2 pins output for rx tx leds and 
	setupUartLeds();
	while (1) // the super loop!
	{		
		processUart0Input();
		checkDigitalInputs();
		while(available()>0)
        {
           processInput();
        }
		newMillis = millis();
		//wait 1 sec if android didin't respond, reset bluetooth
		if (((newMillis-currentMillis)>=responseInterval) && (!getResponseFlag()) )
		{
		   resetBluetooth();
           setResponseFlag(true);
		}
	}
}

