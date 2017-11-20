#include "uart.h"

static volatile uint8_t  UART0_RxBuf[UART0_RX_BUFFER_SIZE];
static volatile uint8_t  UART0_LastRxError;
static volatile uint16_t UART0_RxHead;
static volatile uint16_t UART0_RxTail;

static volatile uint8_t  UART1_RxBuf[UART1_RX_BUFFER_SIZE];
static volatile uint8_t  UART1_LastRxError;
static volatile uint16_t UART1_RxHead;
static volatile uint16_t UART1_RxTail;

static volatile uint8_t  UART2_RxBuf[UART2_RX_BUFFER_SIZE];
static volatile uint8_t  UART2_LastRxError;
static volatile uint16_t UART2_RxHead;
static volatile uint16_t UART2_RxTail;

volatile uint8_t	isArduinoRx0BufferEmpty;
volatile uint8_t	isArduinoRx0BufferOverFlowed;
volatile uint8_t	isArduinoRx1BufferEmpty;
volatile uint8_t	isArduinoRx1BufferOverFlowed;
volatile uint8_t	isArduinoRx2BufferEmpty;
volatile uint8_t	isArduinoRx2BufferOverFlowed;

void setupUartLeds()
{
	SET_BIT(DDRC,6);
	SET_BIT(DDRC,7);
	SET_BIT(PORTC,6);
	SET_BIT(PORTC,7);
	TCCR4B|=(1<<CS40)|(1<<CS41); // clock prescalar =32
}

void UART0_init(uint8_t baudrate)
{	
	UART0_RxHead = 0;
	UART0_RxTail = 0;
	UCSR0A=(1<<U2X);
	UCSR0B=(1<<TXEN)|(1<<RXEN)|(1<<RXCIE);		
	UCSR0C=(1<<UCSZ0)|(1<<UCSZ1);
	switch (baudrate)
	{
		case BAUD_9600:		UBRR0L = 95;break;
		case BAUD_14400:	UBRR0L = 63;break;
		case BAUD_19200:	UBRR0L = 47;break;
		case BAUD_28800:	UBRR0L = 31;break;
		case BAUD_38400:	UBRR0L = 23;break;
		case BAUD_57600:	UBRR0L =  15;break;
		case BAUD_115200:	UBRR0L =  16;break;
		default:			UBRR0L =  16;break;
	}				
}

void UART1_init(uint8_t baudrate)
{
	UART1_RxHead = 0;
	UART1_RxTail = 0;
	UCSR1A=(1<<U2X);;
	UCSR1B=(1<<TXEN)|(1<<RXEN)|(1<<RXCIE);
	UCSR1C=(1<<UCSZ0)|(1<<UCSZ1);
	switch (baudrate)
	{
		case BAUD_9600:		UBRR1L = 95;break;
		case BAUD_14400:	UBRR1L = 63;break;
		case BAUD_19200:	UBRR1L = 47;break;
		case BAUD_28800:	UBRR1L = 31;break;
		case BAUD_38400:	UBRR1L = 23;break;
		case BAUD_57600:	UBRR1L =  15;break;
		case BAUD_115200:	UBRR1L =  16;break;
		default:			UBRR1L =  16;break;
	}
}

void UART2_init(uint8_t baudrate)
{
	UART2_RxHead = 0;
	UART2_RxTail = 0;
	UCSR2A=(1<<U2X);;
	UCSR2B=(1<<TXEN)|(1<<RXEN)|(1<<RXCIE);
	UCSR2C=(1<<UCSZ0)|(1<<UCSZ1);
	switch (baudrate)
	{
		case BAUD_9600:		UBRR2L = 95;break;
		case BAUD_14400:	UBRR2L = 63;break;
		case BAUD_19200:	UBRR2L = 47;break;
		case BAUD_28800:	UBRR2L = 31;break;
		case BAUD_38400:	UBRR2L = 23;break;
		case BAUD_57600:	UBRR2L =  15;break;
		case BAUD_115200:	UBRR2L =  16;break;
		default:			UBRR2L =  16;break;
	}
}
void initUart(uint8_t serialPort,uint8_t baudrate)
{
	switch (serialPort)
	{
		case 0: UART0_init(baudrate); break;
		case 1: UART1_init(baudrate); break;
		case 2: UART2_init(baudrate); break;
		default: UART1_init(baudrate); break;
	}
}
void terminateUart(uint8_t serialPort)
{
	switch (serialPort)
	{
		case 0:
		UCSR0B &=0x07;
		break;
		
		case 1:
		UCSR1B &=0x07;
		break;
		
		case 2:
		UCSR2B &=0x07;
		break;
		
		default:
		/* Your code here */
		break;
	}
}

void writeOnUart0(uint8_t data){
	
	while((UCSR0A&(1<<UDRE))==0)
	{
		
	}
	UDR0=data;
}


void writeOnUart1(uint8_t data){
    while((UCSR1A&(1<<UDRE))==0)
	{
		
	}
	UDR1=data;
}

void writeOnUart2(uint8_t data){
	while((UCSR2A&(1<<UDRE))==0)
	{
		
	}
	UDR2=data;
}

int16_t readFromUart0(){
	uint16_t tmptail;
	uint8_t data;

	if ( UART0_RxHead == UART0_RxTail ) {
		return UART_NO_DATA;   /* no data  */
	}

	/* calculate /store buffer index */
	tmptail = (UART0_RxTail + 1) & UART0_RX_BUFFER_MASK;
	UART0_RxTail = tmptail;
	/* get data from receive buffer */
	data = UART0_RxBuf[tmptail];

	return (UART0_LastRxError << 8) + data;
}

int16_t readFromUart1(){
	uint16_t tmptail;
	uint8_t data;

	if ( UART1_RxHead == UART1_RxTail ) {
		return UART_NO_DATA;   /* no data  */
	}

	/* calculate /store buffer index */
	tmptail = (UART1_RxTail + 1) & UART1_RX_BUFFER_MASK;
	UART1_RxTail = tmptail;
	/* get data from receive buffer */
	data = UART1_RxBuf[tmptail];

	return (UART1_LastRxError << 8) + data;
}

int16_t readFromUart2(){
	uint16_t tmptail;
	uint8_t data;

	if ( UART2_RxHead == UART2_RxTail ) {
		return UART_NO_DATA;   /* no data  */
	}

	/* calculate /store buffer index */
	tmptail = (UART2_RxTail + 1) & UART2_RX_BUFFER_MASK;
	UART2_RxTail = tmptail;
	/* get data from receive buffer */
	data = UART2_RxBuf[tmptail];

	return (UART2_LastRxError << 8) + data;
}

int16_t getAvailableDataCountOnUart0()
{
	
	return (UART0_RX_BUFFER_SIZE + UART0_RxHead - UART0_RxTail) & UART0_RX_BUFFER_MASK;
}

int16_t getAvailableDataCountOnUart1()
{
	
	return (UART1_RX_BUFFER_SIZE + UART1_RxHead - UART1_RxTail) & UART1_RX_BUFFER_MASK;
}

int16_t getAvailableDataCountOnUart2()
{
	
	return (UART2_RX_BUFFER_SIZE + UART2_RxHead - UART2_RxTail) & UART2_RX_BUFFER_MASK;
}

ISR (USART0_RX_vect){
   uint16_t tmphead;
   uint8_t data;
   uint8_t usr;
   uint8_t lastRxError;
   
   /* read UART status register and UART data register */
   usr  = UART0_STATUS;
   data = UART0_DATA;
   
   /* */
   lastRxError = (usr & (_BV(FE)|_BV(DOR)) );
   
   /* calculate buffer index */
   tmphead = ( UART0_RxHead + 1) & UART0_RX_BUFFER_MASK;
   if ((((tmphead + 128) & UART0_RX_BUFFER_MASK)== UART0_RxTail)){
	   isArduinoRx0BufferEmpty = false;
   }
  
   if ( tmphead == UART0_RxTail ) {
	   /* error: receive buffer overflow */
	   lastRxError = UART_BUFFER_OVERFLOW >> 8;
	   isArduinoRx0BufferOverFlowed = true;
	   } else {
	   /* store new index */
	   UART0_RxHead = tmphead;
	   /* store received data in buffer */
	   UART0_RxBuf[tmphead] = data;
   }
   UART0_LastRxError = lastRxError;
}

ISR (USART1_RX_vect){
	uint16_t tmphead;
	uint8_t data;
	uint8_t usr;
	uint8_t lastRxError;

	/* read UART status register and UART data register */
	usr  = UART1_STATUS;
	data = UART1_DATA;

	/* */
	lastRxError = (usr & (_BV(FE)|_BV(DOR)) );

	/* calculate buffer index */
	tmphead = ( UART1_RxHead + 1) & UART1_RX_BUFFER_MASK;

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
	enableTimer4Overflow();
	enableRxLedBlinking();
}
ISR(USART1_TX_vect)
{
	enableTimer4Overflow();
	enableTxLedBlinking();
}
ISR (USART2_RX_vect){
	uint16_t tmphead;
	uint8_t data;
	uint8_t usr;
	uint8_t lastRxError;

	/* read UART status register and UART data register */
	usr  = UART2_STATUS;
	data = UART2_DATA;

	/* */
	lastRxError = (usr & (_BV(FE)|_BV(DOR)) );

	/* calculate buffer index */
	tmphead = ( UART2_RxHead + 1) & UART2_RX_BUFFER_MASK;

	if ( tmphead == UART2_RxTail ) {
		/* error: receive buffer overflow */
		lastRxError = UART_BUFFER_OVERFLOW >> 8;
		} else {
		/* store new index */
		UART2_RxHead = tmphead;
		/* store received data in buffer */
		UART2_RxBuf[tmphead] = data;
	}
	UART2_LastRxError = lastRxError;
}

uint8_t digitalPinToUARTPort(uint8_t pin)
{
	switch (pin)
	{
		case S_PD0:
		return 0;
		break;
		case S_PD1:
		return 0;
		break;
		case S_PD2:
		return 1;
		break;
		case S_PD3:
		return 1;
		break;
		case S_PE2:
		return 2;
		break;
		case S_PE3:
		return 2;
		break;
		default:
		return -1;
		break;
	}
}

uint8_t getIsArduinoRx0BufferEmptyFlag()
{
	return isArduinoRx0BufferEmpty;
}

void setIsArduinoRx0BufferEmptyFlag(uint8_t state)
{
	isArduinoRx0BufferEmpty = state;
}


uint8_t getIsArduinoRx0BufferOverFlowedFlag()
{
	return isArduinoRx0BufferOverFlowed;
}

void setIsArduinoRx0BufferOverFlowedFlag(uint8_t state)
{
	isArduinoRx0BufferOverFlowed = state;
}