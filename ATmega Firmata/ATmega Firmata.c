/*
 * ATmega_Firmata.c
 *
 * Created: 2/12/2013 9:43:21 AM
 *  Author: iMustafa
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "sys.h"
#include "gpio.h"
#include "uart.h"
#include "CommonMacros.h"
#include "1sheelds_functions.h"



int main(void)
{
	//t_SetPortCfg cfg;
	//UartInit(0,9600);
	//SREG|=(1<<7);
	//cfg.Portdir=0x00;
	//cfg.Portdir |=(1<<1);
	//cfg.initValue=0x02;
	//cfg.pID=C;
	//GPIO_Cfg(&cfg);
	
	//t_stPort* stport;
	//stport=(t_stPort *)cfg.pID;
	
	
	
	pinMode(10, OUTPUT);
	
    while(1)
    {
       
	    digitalWrite(10, HIGH);
	    _delay_ms(1000);
	    digitalWrite(10, LOW);
	    _delay_ms(1000);
	   
	   /* //UartTx0('S');
		 while(getuartRx0Flag()==0);
		 
			 UartTx0(UartRx0());
		 			
		//GPIO_setPort(UartRx0(),cfg.pID);
		//GPIO_setPort(i++,cfg.pID);
		_delay_ms(100);
		
		
		TOG_BIT((stport->portOutReg),5);
		*/
	   
	  // GPIO_setPin(1,cfg.pID,1);
	   
	   
	 
    }
	
}