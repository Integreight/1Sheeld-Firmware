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
  	
	 // uint8 duty = 0;
	//DDRB=0xff;
	//t_SetPortCfg cfg;
	//UartInit(0,9600);
	//SREG|=(1<<7);
	//cfg.Portdir=0x00;
	//cfg.Portdir |=(1<<1);
	//cfg.initValue=0x02;
	//cfg.pID=A;
	//GPIO_Cfg(&cfg);
	
	//t_stPort* stport;
	//stport=(t_stPort *)cfg.pID;
	
	
	

	/*
	pinMode(1, OUTPUT);
	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
	pinMode(8, OUTPUT);
	pinMode(9, OUTPUT);
	pinMode(10, OUTPUT);
	pinMode(11, OUTPUT);
	pinMode(12, OUTPUT);
	pinMode(13, OUTPUT);
	
	pinMode(14, OUTPUT);*/
    //DDRB=0xff;
	//PORTB=0x00;
	//pwm_Setup(TIMER_00);
	
	//TCCR0|=(1<<WGM00)|(1<<WGM01)|(1<<CS00)|(1<<COM01);
	pinMode(4,OUTPUT);
	UartInit(0,BAUD_57600);
	sei();
    while(1)
    {
		if (getuartRx0Flag()!=0)
		{
			UartTx0(UartRx0());
			 writePort(0,0xff,0xff);
			 
		}
		_delay_ms(100);
		writePort(0,0x00,0xff); 
		_delay_ms(100);
		  
		 //  writePort(0,0x00,0xff);
		//	_delay_ms(100);
			
	}
	
	
	
	
}