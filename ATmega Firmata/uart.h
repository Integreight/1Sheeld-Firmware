/*
 * uart.h
 *
 * Created: 03/06/2012 12:46:36 Õ
 *  Author: HP
 */ 


#ifndef UART_H_
#define UART_H_
#include "CommonMacros.h"
#include "sys.h"

#define UART_RX0_INTERRUPT ENABLED
#define UART_RX1_INTERRUPT ENABLED 
#define BUFFER_SIZE 64

// baud rate register values for U2X=1
#define BAUD_4800   0x1A0
#define BAUD_9600   0xCF
#define BAUD_14400  0x8A
#define BAUD_19200  0x67
#define BAUD_28800  0x44
#define BAUD_38400  0x33
#define BAUD_57600  0x22
#define BAUD_76800  0x19
#define BAUD_115200 0x10

//#include "sys.h"

void UartInit(uint8 serialPort,uint16 baudRate);
void UartTx0(unsigned char data);
void UartTx1(unsigned char data);
int  UartRx0();
int  UartRx1();
#if UART_RX0_INTERRUPT == ENABLED
    int getuartRx1Flag();
    int getuartRx0Flag();
#endif




#endif /* UART_H_ */