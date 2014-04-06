/*
 * ATmega_Firmata.c
 *
 * Created: 2/12/2013 9:43:21 AM
 *  Author: iMustafa
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
unsigned long responseInterval =2000 ;
bool resetBtFlag ;
int freeRam () {
	extern int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}



int main(void)
{
	// for millis fn 
	resetBtFlag=true;
	TCCR0=(1<<CS00)|(1<<CS01);
	SET_BIT(TIMSK,TOIE0);
	sei(); // enable global interrupt
	Firmata.begin();
	Firmata.systemResetCallback();  // reset to default config
	unusedPinsAsOutput();
	Firmata.requestBluetoothReset();
	currentMillis=millis();
	//make 2 pins output for rx tx leds and 
	SET_BIT(DDRA,6);
	SET_BIT(DDRA,7);
	SET_BIT(PORTA,6);
	SET_BIT(PORTA,7);
	TCCR2|=(1<<CS20)|(1<<CS21); // clock prescalar =32
	
	while (1) // the super loop!
	{
		
		Firmata.processUart0Input();
		Firmata.checkDigitalInputs();
		while(Firmata.available()>0)
        {
           Firmata.processInput();
        }
		newMillis = millis();
		//wait 1 sec if android didin't respond, reset bluetooth
		if (((newMillis-currentMillis)>=responseInterval) && (!Firmata.didRespond()) && resetBtFlag)
		{
		   resetBluetooth();
		  // currentMillis+=responseInterval;
		   resetBtFlag=false;
		}

		if(Firmata.isPulseInEnabled)
		{
			pinMode(Firmata.pinPWM,INPUT);
			unsigned int value =readPWM(Firmata.pinPWM);
            Firmata.sendSysexDataByte(PULSE_IN_DATA,value);
			
		}
	}


}

