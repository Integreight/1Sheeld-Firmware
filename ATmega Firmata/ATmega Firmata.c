/*
 * ATmega_Firmata.c
 *
 * Created: 2/12/2013 9:43:21 AM
 *  Author: iMustafa
 */ 

#include <avr/io.h>
#include "gpio.h"
#include "uart.h"
#include "CommonMacros.h"
#include <util/delay.h>


int main(void)
{
	uint8  i=0; 
	t_SetPortCfg cfg;
	UartInit(0,9600);
	
	cfg.Portdir=OUTPUT;
	cfg.initValue=0x00;
	cfg.pID=C;
	GPIO_Cfg(&cfg);
	
	t_stPort* stport;
	stport=(t_stPort *)cfg.pID;
	
	
    while(1)
    {
        //UartTx0('S');
		UartTx0(UartRx0());
		//GPIO_setPort(UartRx0(),cfg.pID);
		//GPIO_setPort(i++,cfg.pID);
		_delay_ms(100);
		
		
		TOG_BIT((stport->portOutReg),5);
		
    }
	
}