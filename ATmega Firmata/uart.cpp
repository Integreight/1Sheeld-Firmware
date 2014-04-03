/*
 * uart.c
 *
 * Created: 03/06/2012 12:46:48 Õ
 *  Author: HP
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


void UartInit(uint8 serialPort,uint16 baudRate){
	
	
			//UBRR0H=0x00;
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
			/*
			UBRR0L = (byte) (baudRate &0x00ff) ;
			UBRR0H = (byte)((baudRate &0xff00)>>8);
				*/
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
		  /*  UBRR1L = (byte) (baudRate &0x00ff) ;
		    UBRR1H =(byte)((baudRate &0xff00)>>8);
             */
		    UBRR1L= 3; // 115200 single speed
			
		break;
		
		default:
			break;
		
	}
	
}

void UartEnd(uint8 serialPort)
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

void UartTx0(unsigned char data){
	
	
	
	while((UCSR0A&(1<<UDRE0))==0)
	{
		
	}
	UDR0=data;
}


void UartTx1(uint8_t data){
    while((UCSR1A&(1<<UDRE1))==0)
	{
		
	}
	UDR1=data;
}

#if UART_RX0_INTERRUPT == ENABLED

uint16_t UartRx0(){
	uint16_t tmptail;
	uint8_t data;

	if ( UART0_RxHead == UART0_RxTail ) {
		return UART_NO_DATA;   /* no data  */
	}

	/* calculate /store buffer index */
	tmptail = (UART0_RxTail + 1) & UART0_RX0_BUFFER_MASK;
	UART0_RxTail = tmptail;

	/* get data from receive buffer */
	data = UART0_RxBuf[tmptail];

	return (UART0_LastRxError << 8) + data;
}


uint16_t getuartRx0Flag(){
	
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



int UartRx1(){
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


int getuartRx1Flag(){
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
	timer_Ovf_enable();
	enableRxLed();
}
ISR(USART1_TXC_vect)
{
		timer_Ovf_enable();
		enableTxLed();
}
#else
int UartRx0(){
	
	while ( (UCSR0A & (1<<RXC0))==0);
	return UDR0;
}

int UartRx1(){
	
	
	while ( !(UCSR1A & (1<<RXC1)) );
	return UDR1;
}

#endif
