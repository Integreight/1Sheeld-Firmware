/*

  Project:       1Sheeld Firmware
  File:          uart.h

  Compiler:      Arduino avr-gcc 4.3.2

  Author:        Integreight
  
  Date:          2014.5

*/

/**
 * @file Uart.h
 * @brief UART driver for handling the Serial Data  	
 * @author Integreight
 * @version 1.1
 */ 

#ifndef UART_H_
#define UART_H_
#include "CommonMacros.h"
#include "sys.h"

#define UART_RX0_INTERRUPT ENABLED
#define UART_RX1_INTERRUPT ENABLED 
#define UART_RX1_BUFFER_SIZE     64
#define UART_RX1_BUFFER_MASK     ( UART_RX1_BUFFER_SIZE - 1)
#define UART1_STATUS   UCSR1A
#define UART1_DATA     UDR1
#define UART0_RX0_BUFFER_SIZE    256
#define UART0_RX0_BUFFER_MASK    ( UART0_RX0_BUFFER_SIZE - 1)
#define UART0_STATUS   UCSR0A
#define UART0_DATA     UDR0

//Error codes
#define UART_NO_DATA          -1              /**< no receive data available   */
#define UART_BUFFER_OVERFLOW  -2              /**< receive ringbuffer overflow */


/**
 * @brief Initialize UART control and status registers and buffers.
 * @param serialPort specify serialport "0" or "1"
 * @param baudRate specify baudrate of the serial data 
	(i:e check the UART Example table of the Controller)  
 * @return None 
 */
void UartInit(uint8 serialPort);
/**
 * @brief Ends the UART Functionality.
 * @param serialPort specify serialport "0" or "1"  
 * @return None 
 */
void UartEnd(uint8 serialPort);
/**
 * @brief Transmits the byte placed in it's buffer0.
 * @param data data to be placed in the transmitter's buffer0  
 * @return None 
 */
void UartTx0(unsigned char data);
/**
 * @brief Transmits the byte placed in it's buffer1.
 * @param data data to be placed in the transmitter's buffer1.  
 * @return None 
 */
void UartTx1(uint8_t data);
/**
 * @brief Receives byte from it's buffer0.
 * @param None.  
 * @return the data received in bytes.  
 */
int  UartRx0();
/**
 * @brief Receives byte from it's buffer1.
 * @param None.  
 * @return the data received in bytes.  
 */
int  UartRx1();
#if UART_RX0_INTERRUPT == ENABLED
	/**
	  * @brief Flag to check if a certain data came from the outside world to buffer1.
	  * @param None.  
	  * @return A boolean whether "1" true or "0" false.  
	  */
    int getuartRx1Flag();
	/**
	  * @brief Flag to check if a certain data came from the outside world to buffer0.
	  * @param None.  
	  * @return A boolean whether "1" true or "0" false.  
	  */
    int getuartRx0Flag();
#endif

#endif /* UART_H_ */