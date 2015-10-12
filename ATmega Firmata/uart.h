/*

  Project:       1Sheeld Firmware
  File:          uart.h

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
  
  Date:          2014.5

*/

/**
 * @file Uart.h
 * @brief UART driver for handling the serial data on ATmega162 uart modules	
 * @author Integreight
 * @version 1.1
 */ 

#ifndef UART_H_
#define UART_H_
#include "CommonMacros.h"
#include "sys.h"

#define UART_RX0_INTERRUPT ENABLED
#define UART_RX1_INTERRUPT ENABLED 
#define UART_RX1_BUFFER_SIZE     256
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
 * @brief Initialize the serial port, it's status registers and buffers.
 * @param serialPort specify which serial port to initialize 0 or 1
 * @return None 
 */
void initUart(uint8 serialPort);
/**
 * @brief Disables the serial port, it's status registers and buffers. and returns rx and tx pins to their defaults.
 * @param serialPort specify which serial port to disable 0 or 1 
 * @return None 
 */
void terminateUart(uint8 serialPort);
/**
 * @brief Transmits a byte to serial port 0.
 * @param data the byte to be sent  
 * @return None 
 */
void writeOnUart0(uint8_t data);
/**
 * @brief Transmits a byte to serial port 1.
 * @param data the byte to be sent  
 * @return None 
 */
void writeOnUart1(uint8_t data);
/**
 * @brief Receives a byte from serial port 0.
 * @param None.  
 * @return the received byte.  
 */
int  readFromUart0();
/**
 * @brief Receives a byte from serial port 1.
 * @param None.  
 * @return the received byte.  
 */
int  readFromUart1();
#if UART_RX0_INTERRUPT == ENABLED
	/**
	  * @brief Gets the count of the available data in the buffer of serial 1.
	  * @param None.  
	  * @return the data count.  
	  */
    int getAvailableDataCountOnUart1();
	/**
	  * @brief Gets the count of the available data in the buffer of serial 0.
	  * @param None.  
	  * @return the data count.  
	  */
    int getAvailableDataCountOnUart0();
#endif

#endif /* UART_H_ */