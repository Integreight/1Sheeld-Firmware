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

int freeRam () {
	extern int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

int main(void)
{
	//Firmata.setFirmwareVersion(FIRMATA_MAJOR_VERSION, FIRMATA_MINOR_VERSION);
	Firmata.begin();
	Firmata.systemResetCallback();  // reset to default config
	
	// for rx tx leds initialization
	SET_BIT(DDRA,6);
	SET_BIT(DDRA,7);
	SET_BIT(PORTA,6);
	SET_BIT(PORTA,7);
	
	TCCR2|=(1<<CS20)|(1<<CS21);

	sei();

	while (1) // the super loop!
	{
		
		int availableData=serial0_Avilable();
		if(availableData>0){
			byte arr[availableData];
			for(int i=0;i<availableData;i++){
				arr[i]=UartRx0();
			}
			Firmata.sendSysex(UART_DATA,availableData,arr);
		}

/*
		if(isPulseInEnabled)
		{
			
			byte analogData[2];
			long value =convertPWM->Convert();
			analogData [0]=(value & B01111111);
			analogData[1]=(value >> 7 & B01111111);
			//Firmata.sendSysex(PULSE_IN_DATA,2,analogData); // convert to analog and send it via serial comm
			Firmata.sendByte(START_SYSEX);
			Firmata.sendByte(0x88);//command
			Firmata.sendByte(analogData [0]);
			Firmata.sendByte(analogData [1]); // convert to analog and send it via serial comm
			Firmata.sendByte(END_SYSEX);
			
			
		}*/

		Firmata.checkDigitalInputs();
		while(Firmata.available()>0)
          {
             
           Firmata.processInput();
          }
          


  }

}