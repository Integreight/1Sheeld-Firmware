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



int main(void)
{

	 // global interrupt enable 
	//Firmata.setFirmwareVersion(FIRMATA_MAJOR_VERSION, FIRMATA_MINOR_VERSION);
	
	
	//Firmata.begin();
	//Firmata.systemResetCallback();  // reset to default config
	SET_BIT(DDRA,6);
	TCCR2|=(1<<CS20);
	UartInit(1,BAUD_57600);
	
	
	DDRC=0xff;
	PORTC=0x01;
	sei();
	unsigned int val =0;
	while (1) // the super loop!
	{
		/*
		while (serial0_Avilable()>0)
		{
		Firmata.sendSysexDataByte(UART_DATA,UartRx0());
		}


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

	/*	Firmata.checkDigitalInputs();
		while(Firmata.available()>0)
          {
             
           Firmata.processInput();
          }
          
		*/
	    
	 // analogWrite(9,val);
   //  analogWrite(11,val);


		if(getuartRx1Flag()>0)		
		{	UartTx1(UartRx1());
			
			//TOG_BIT(PORTA,1);
		}
		val+=5;
		if (val>255)
		{
			val=0;
		}
		//analogWrite(9,166);
  }

}