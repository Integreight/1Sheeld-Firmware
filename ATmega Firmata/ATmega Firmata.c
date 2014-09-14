/*

  Project:       1Sheeld Firmware 
  File:          ATmega Firmata.cpp

  Compiler:      Arduino avr-gcc 4.3.2

  Author:        Integreight
                 
  Date:          2014.5

*/

#define  F_CPU 7372800UL //
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "gpio.h"
#include "uart.h"
#include "1sheelds_functions.h"
#include "pwm.h"
#include "firmata.h"
#include "timers.h"

unsigned long currentMillis;        // store the current value from millis()
unsigned long newMillis;
unsigned long responseInterval =200UL ;

void millis_setup()
{
	TCCR0=(1<<CS00)|(1<<CS01);
	SET_BIT(TIMSK,TOIE0);
}

void UartLedSetup()
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
	millis_setup();
	sei(); // enable global interrupt
	begin();
	systemResetCallback();  // reset to default config
	unusedPinsAsOutput();
	requestBluetoothReset();
	currentMillis=millis();
	//make 2 pins output for rx tx leds and 
	UartLedSetup();
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

