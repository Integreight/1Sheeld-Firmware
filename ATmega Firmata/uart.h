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

//#include "sys.h"

void UartInit(uint8 serialPort,uint16 baudRate);
void UartTx0(unsigned char data);
void UartTx1(unsigned char data);
unsigned char UartRx0();
unsigned char UartRx1();
unsigned char getuartRx1Flag();
unsigned char getuartRx0Flag();




#endif /* UART_H_ */