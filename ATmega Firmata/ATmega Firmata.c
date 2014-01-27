/*
 * ATmega_Firmata.c
 *
 * Created: 2/12/2013 9:43:21 AM
 *  Author: iMustafa
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "sys.h"
#include "gpio.h"
#include "uart.h"
#include "CommonMacros.h"
#include "1sheelds_functions.h"
#include "pwm.h"


int main(void)
{
  	
	 
	UartInit(0,BAUD_9600);
	sei();
	
    while(1)
    {
		
			if(serial0_Avilable() > 0)
			{
				UartTx0(UartRx0());	
			}
	}
	
	
	
	
}