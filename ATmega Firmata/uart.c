/*
 * uart.c
 *
 * Created: 03/06/2012 12:46:48 Õ
 *  Author: HP
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
//#include "uart.h"
#include "sys.h"

#if UART_RX0_INTERRUPT == ENABLED
volatile unsigned char uartRx0Flag=0;
volatile unsigned char uartRx0databuffer[10];
volatile signed char index0=0;
#endif 

#if UART_RX1_INTERRUPT == ENABLED
volatile unsigned char uartRx1Flag=0;
volatile unsigned char uartRx1databuffer[10];
volatile signed char index1=0;
#endif 

void UartInit(unsigned char serialPort,uint16 baudRate){
	
	
			//UBRR0H=0x00;

	
	switch(serialPort)
	{
		case 0:
			#if UART_RX0_INTERRUPT == DISABLED	
		
			UBRR0L=0x33;
			UBRR0H=0x00;
			UCSR0B=UCSR0B|(1<<TXEN0)|(1<<RXEN0);		
			UCSR0C=UCSR0C|(1<<URSEL0)|(1<<UCSZ00)|(1<<UCSZ01);
			
			#else
		
			UBRR0L=0x33;
			UBRR0H=0x00;
			UCSR0B=UCSR0B|(1<<TXEN0)|(1<<RXEN0)|(1<<RXCIE0);		
			UCSR0C=UCSR0C|(1<<URSEL0)|(1<<UCSZ00)|(1<<UCSZ01);
		
			#endif
		break;
		
		case 1:
			#if UART_RX1_INTERRUPT == DISABLED
			UBRR1L=0x33;
			UCSR1B=UCSR1B|(1<<TXEN1)|(1<<RXEN1);		
			UCSR1C=UCSR1C|(1<<URSEL1)|(1<<UCSZ10)|(1<<UCSZ11);
			#else
			UBRR1L=0x33;
			UCSR1B=UCSR1B|(1<<TXEN1)|(1<<RXEN1)|(1<<RXCIE1);		
			UCSR1C=UCSR1C|(1<<URSEL1)|(1<<UCSZ10)|(1<<UCSZ11);
			#endif 
		break;
		
		default:
			break;		
	}
	
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

#if UART_RX0_INTERRUPT == ENABLED

unsigned char UartRx0(){
	
	unsigned char data;
	unsigned int i;
	data=uartRx0databuffer[0];
	uartRx0Flag=0;
	
	for(i=0;i<index0;i++)
		uartRx0databuffer[i]=uartRx0databuffer[i+1];
	
	index0--;	
	if(index0<0)
		index0=0;
		
	return data;
}


unsigned char getUartRx0Flag(){
	return uartRx0Flag;
	
}
ISR (USART0_RXC_vect){
	
	uartRx0Flag=1;
	uartRx0databuffer[index0]=UDR0;
	index0++;
}



unsigned char UartRx1(){
	
	unsigned char data;
	unsigned int i;
	data=uartRx1databuffer[0];
	uartRx1Flag=0;
	
	for(i=0;i<index1;i++)
		uartRx1databuffer[i]=uartRx1databuffer[i+1];
	
	index1--;	
	if(index1<0)
		index1=0;
		
	return data;
}


unsigned char getuartRx1Flag(){
	return uartRx1Flag;
	
}

ISR (USART1_RXC_vect){
	
	uartRx0Flag=1;
	uartRx0databuffer[index0]=UDR0;
	index0++;
}

#else
unsigned char UartRx0(){
	
	while ( (UCSR0A & (1<<RXC0))==0);
	return UDR0;
}

unsigned char UartRx1(){
	
	
	while ( !(UCSR1A & (1<<RXC1)) );
	return UDR1;
}

#endif
