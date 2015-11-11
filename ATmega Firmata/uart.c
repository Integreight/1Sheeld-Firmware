/*

  Project:       1Sheeld Firmware 
  File:          uart.cpp

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
                 
  Date:          2014.5

*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "timers.h"



#if UART_RX0_INTERRUPT == ENABLED
static volatile uint8_t  UART0_RxBuf[UART0_RX0_BUFFER_SIZE];
static volatile uint8_t  UART0_LastRxError;
static volatile uint16_t UART0_RxHead;
static volatile uint16_t UART0_RxTail;
#endif 

#if UART_RX1_INTERRUPT == ENABLED
static volatile uint8_t  UART1_RxBuf[UART_RX1_BUFFER_SIZE];
static volatile uint8_t  UART1_LastRxError;
static volatile uint16_t UART1_RxHead;
static volatile uint16_t UART1_RxTail;
#endif 


void initUart(uint8 serialPort){
	
    UART1_RxHead = 0;
	UART1_RxTail = 0;
	UART0_RxHead = 0;
	UART0_RxTail = 0;
	
	switch(serialPort)
	{
		case 0:
		    UCSR0A=0x00;
		  	#if UART_RX0_INTERRUPT == DISABLED	  
			UCSR0B=(1<<TXEN0)|(1<<RXEN0);		
			UCSR0C=(1<<URSEL0)|(1<<UCSZ00)|(1<<UCSZ01);
			#else
		    UCSR0B=(1<<TXEN0)|(1<<RXEN0)|(1<<RXCIE0);		
			UCSR0C=(1<<URSEL0)|(1<<UCSZ00)|(1<<UCSZ01);
			#endif
			UBRR0L= 3; // 115200 single speed
			
		break;
		
		case 1:
		    UCSR1A=0x00;
		 	#if UART_RX1_INTERRUPT == DISABLED
			UCSR1B=(1<<TXEN1)|(1<<RXEN1);		
			UCSR1C=(1<<URSEL1)|(1<<UCSZ10)|(1<<UCSZ11);
			#else
			UCSR1B=(1<<TXEN1)|(1<<RXEN1)|(1<<RXCIE1)|(1<<TXCIE1);		
			UCSR1C=(1<<URSEL1)|(1<<UCSZ10)|(1<<UCSZ11);
			#endif 
		    UBRR1L= 3; // 115200 single speed
			
		break;
		
		default:
			break;
		
	}
	
}

void terminateUart(uint8 serialPort)
{
	if (serialPort==0)
	{
		UCSR0B &=0x07;
	}
	else
	{
		UCSR1B &=0x07;
	}

	
}

void writeOnUart0(uint8_t data){
	
	
	
	while((UCSR0A&(1<<UDRE0))==0)
	{
		
	}
	UDR0=data;
}


void writeOnUart1(uint8_t data){
    while((UCSR1A&(1<<UDRE1))==0)
	{
		
	}
	UDR1=data;
}

#if UART_RX0_INTERRUPT == ENABLED

int readFromUart0(){
	uint16_t tmptail;
	uint8_t data;

	if ( UART0_RxHead == UART0_RxTail ) {
		return UART_NO_DATA;   /* no data  */
	}

	/* calculate /store buffer index */
	tmptail = (UART0_RxTail + 1) & UART0_RX0_BUFFER_MASK;
	UART0_RxTail = tmptail;
	
	if (((tmptail + 1) & UART0_RX0_BUFFER_MASK)== UART0_RxHead && arduinoStopped)
	{
		sendArduinoToSendFlag = true;
	}
	/* get data from receive buffer */
	data = UART0_RxBuf[tmptail];

	return (UART0_LastRxError << 8) + data;
}


int getAvailableDataCountOnUart0(){
	
	return (UART0_RX0_BUFFER_SIZE + UART0_RxHead - UART0_RxTail) & UART0_RX0_BUFFER_MASK;
}

ISR (USART0_RXC_vect){
   uint16_t tmphead;
   uint8_t data;
   uint8_t usr;
   uint8_t lastRxError;
   
   /* read UART status register and UART data register */
   usr  = UART0_STATUS;
   data = UART0_DATA;
   
   /* */
   lastRxError = (usr & (_BV(FE0)|_BV(DOR0)) );
   
   /* calculate buffer index */
   tmphead = ( UART0_RxHead + 1) & UART0_RX0_BUFFER_MASK;
   
   if (((tmphead + 128) & UART0_RX0_BUFFER_MASK)== UART0_RxTail && !arduinoStopped){
	   sendArduinoToStopFlag = true;
   }
   
   if ( tmphead == UART0_RxTail ) {
	   /* error: receive buffer overflow */
	   lastRxError = UART_BUFFER_OVERFLOW >> 8; 
	   } else {
	   /* store new index */
	   UART0_RxHead = tmphead;
	   /* store received data in buffer */
	   UART0_RxBuf[tmphead] = data;
   }
   UART0_LastRxError = lastRxError;	
}



int readFromUart1(){
	uint16_t tmptail;
	uint8_t data;

	if ( UART1_RxHead == UART1_RxTail ) {
		return UART_NO_DATA;   /* no data available */
	}

	/* calculate /store buffer index */
	tmptail = (UART1_RxTail + 1) & UART_RX1_BUFFER_MASK;
	UART1_RxTail = tmptail;

	/* get data from receive buffer */
	data = UART1_RxBuf[tmptail];

	return (UART1_LastRxError << 8) + data;
}


int getAvailableDataCountOnUart1(){
	return (UART_RX1_BUFFER_SIZE + UART1_RxHead - UART1_RxTail) & UART_RX1_BUFFER_MASK;
}

ISR (USART1_RXC_vect){
	uint16_t tmphead;
	uint8_t data;
	uint8_t usr;
	uint8_t lastRxError;

	/* read UART status register and UART data register */
	usr  = UART1_STATUS;
	data = UART1_DATA;

	/* */
	lastRxError = (usr & (_BV(FE1)|_BV(DOR1)) );

	/* calculate buffer index */
	tmphead = ( UART1_RxHead + 1) & UART_RX1_BUFFER_MASK;

	if ( tmphead == UART1_RxTail ) {
		/* error: receive buffer overflow */
		lastRxError = UART_BUFFER_OVERFLOW >> 8;
		} else {
		/* store new index */
		UART1_RxHead = tmphead;
		/* store received data in buffer */
		UART1_RxBuf[tmphead] = data;
	}
	UART1_LastRxError = lastRxError;
	enableTimerOverflow();
	enableRxLedBlinking();
}
ISR(USART1_TXC_vect)
{
		enableTimerOverflow();
		enableTxLedBlinking();
}
#else
int readFromUart0(){
	
	while ( (UCSR0A & (1<<RXC0))==0);
	return UDR0;
}

int readFromUart1(){
	
	
	while ( !(UCSR1A & (1<<RXC1)) );
	return UDR1;
}

#endif
