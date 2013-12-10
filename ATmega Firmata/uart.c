/*
 * uart.c
 *
 * Created: 03/06/2012 12:46:48 Õ
 *  Author: HP
 */ 
#include <avr/io.h>
//#include <avr/interrupt.h>
//#include "uart.h"
#include "sys.h"

unsigned char flag=0;
unsigned char databuffer;

void UartInit(unsigned char serialPort,uint16 baudRate){
	
	
			//UBRR0H=0x00;
				
		
	switch(serialPort)
	{
		
		
		case 0:
			UBRR0L=0x33;
			UBRR0H=0x00;
			UCSR0B=UCSR0B|(1<<TXEN0)|(1<<RXEN0);		
			UCSR0C=UCSR0C|(1<<URSEL0)|(1<<UCSZ00)|(1<<UCSZ01);
		break;
		case 1:
			UBRR1L=0x33;
			UCSR1B=UCSR1B|(1<<TXEN1)|(1<<RXEN1);		
			UCSR1C=UCSR1C|(1<<URSEL1)|(1<<UCSZ10)|(1<<UCSZ11);
		break;
		default:
		break;		
	}
	/*DDRD=DDRD|(1<<1);
	DDRD=DDRD&(~(1<<0));
	UBRRL=0x33;  //baud rate 9600 8MHZ
	UCSRB=UCSRB|(1<<TXEN)|(1<<RXEN)|(1<<RXCIE);
	UCSRC=UCSRC|(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
	*/
	
//	UBRRL=0x33;
	
	
	
}

void UartTx0(unsigned char data){
	
	while((UCSR0A&(1<<UDRE0))==0)
	{
		
	}
	UDR0=data;
	
}

void UartTx1(unsigned char data){
	
	while((UCSR1A&(1<<UDRE1))==0)
	{
		
	}
	UDR1=data;
	
}


unsigned char UartRx0(){
	
	while ( (UCSR0A & (1<<RXC0))==0);
	return UDR0;
}

unsigned char UartRx1(){
	
	
	while ( !(UCSR1A & (1<<RXC1)) );
	return UDR1;
}


