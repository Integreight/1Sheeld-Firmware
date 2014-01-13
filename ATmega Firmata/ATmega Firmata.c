/*
 * ATmega_Firmata.c
 *
 * Created: 2/12/2013 9:43:21 AM
 *  Author: iMustafa
 */ 
//#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "sys.h"
#include "gpio.h"
#include "uart.h"
#include "CommonMacros.h"
#include "1sheelds_functions.h"
#include "pwm.h"

int main(void)
{
	uint8 duty = 255;
	DDRB=0xff;
	/*t_SetPortCfg cfg;
	//UartInit(0,9600);
	//SREG|=(1<<7);
	//cfg.Portdir=0x00;
	//cfg.Portdir |=(1<<1);
	//cfg.initValue=0x02;
	cfg.pID=A;
	//GPIO_Cfg(&cfg);
	
	//t_stPort* stport;
	//stport=(t_stPort *)cfg.pID;
	
	
	

	
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
    //pwm_Setup(TIMER_00);
	TCCR0|=(1<<WGM00)|(1<<WGM01)|(1<<CS00)|(1<<COM01);
    while(1)
    {
       

	  /*  digitalWrite(14, HIGH);
		
	    
		
		digitalWrite(4, LOW);
		digitalWrite(1, HIGH);
		digitalWrite(2, HIGH);
		digitalWrite(3, HIGH);
		digitalWrite(4, HIGH);
		digitalWrite(5, HIGH);
		digitalWrite(6, HIGH);
		digitalWrite(7, HIGH);
		digitalWrite(8, HIGH);
		digitalWrite(9, HIGH);
		digitalWrite(10, HIGH);
		digitalWrite(11, HIGH);
		digitalWrite(12, HIGH);
		digitalWrite(13, HIGH);
		//GPIO_setPin(0,cfg.pID,7);
	     _delay_ms(1000);
	    digitalWrite(1, LOW);
	    digitalWrite(2, LOW);
	    digitalWrite(3, LOW);
	    digitalWrite(4, LOW);
		digitalWrite(5, LOW);
	    digitalWrite(6, LOW);
	    digitalWrite(7, LOW);
	    digitalWrite(8, LOW);
	    digitalWrite(9, LOW);
	    digitalWrite(10, LOW);
	    digitalWrite(11, LOW);
	    digitalWrite(12, LOW);
	    digitalWrite(13, LOW);
		digitalWrite(14, LOW);
		//GPIO_setPin(1,cfg.pID,7);
	   // _delay_ms(1000);
	   _delay_ms(1000);
	    //UartTx0('S');
		// while(getuartRx0Flag()==0);

			// UartTx0(UartRx0());
		 			
		//GPIO_setPort(UartRx0(),cfg.pID);
		//GPIO_setPort(i++,cfg.pID);
		//GPIO_setPin(0,cfg.pID,1);
		//_delay_ms(1000);
		
		
		//TOG_BIT((stport->portOutReg),5);
		
	   
	//   GPIO_setPin(1,cfg.pID,1);
	  // _delay_ms(1000);
	   */
	 
		//pwm_SetDutyCycle(0,TIMER_00);
		_delay_ms(100);
		OCR0=duty++;

	}
	
	
	
	
}