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
void UartInit(uint8 serialPort,uint16 baudRate);
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
uint16_t  UartRx0();
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
    uint16_t getuartRx0Flag();
#endif

#endif /* UART_H_ */