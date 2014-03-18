/*
 * ATmega_Firmata.c
 *
 * Created: 2/12/2013 9:43:21 AM
 *  Author: iMustafa
 */ 

#define  F_CPU 16000000UL //
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "gpio.h"
#include "uart.h"
#include "1sheelds_functions.h"
#include "pwm.h"
#include "firmata.h"
#include <util/delay.h>

unsigned long val=0;
int freeRam () {
	extern int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}



int main(void)
{
	
	Firmata.begin();
	Firmata.systemResetCallback();  // reset to default config
	
	//make 2 pins output for rx tx leds and 
	SET_BIT(DDRA,6);
	SET_BIT(DDRA,7);
	SET_BIT(PORTA,6);
	SET_BIT(PORTA,7);
	TCCR2|=(1<<CS20)|(1<<CS21); // clock prescalar =32

	sei(); // enable global interrupt
	while (1) // the super loop!
	{
		
		Firmata.processUart0Input();
		Firmata.checkDigitalInputs();
		while(Firmata.available()>0)
        {
           Firmata.processInput();
         }
      
		

		if(Firmata.isPulseInEnabled)
		{
			pinMode(Firmata.pinPWM,INPUT);
			unsigned int value =readPWM(Firmata.pinPWM);
            Firmata.sendSysexDataByte(PULSE_IN_DATA,value);
			
		}
	}

}

