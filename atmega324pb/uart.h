#ifndef UART_H_
#define UART_H_
#include "stdint.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "macros.h"
#include "timers.h"

#define BAUD_9600	0x01
#define BAUD_14400	0x02
#define BAUD_19200	0x03
#define BAUD_28800	0x04
#define BAUD_38400	0x05
#define BAUD_57600	0x06
#define BAUD_115200	0x07

#define UART_RX0_INTERRUPT ENABLED
#define UART_RX1_INTERRUPT ENABLED 
#define UART_RX2_INTERRUPT ENABLED

#define UART0_RX_BUFFER_SIZE     256
#define UART0_RX_BUFFER_MASK     ( UART0_RX_BUFFER_SIZE - 1)
#define UART1_RX_BUFFER_SIZE     256
#define UART1_RX_BUFFER_MASK     ( UART1_RX_BUFFER_SIZE - 1)
#define UART2_RX_BUFFER_SIZE     256
#define UART2_RX_BUFFER_MASK     ( UART2_RX_BUFFER_SIZE - 1)

#define UART0_STATUS   UCSR0A
#define UART0_DATA     UDR0
#define UART1_STATUS   UCSR1A
#define UART1_DATA     UDR1
#define UART2_STATUS   UCSR2A
#define UART2_DATA     UDR2

//Error codes
#define UART_NO_DATA          -1              /**< no receive data available   */
#define UART_BUFFER_OVERFLOW  -2              /**< receive ringbuffer overflow */

void UART0_init(uint8_t baudrate);
void UART1_init(uint8_t baudrate);
void UART2_init(uint8_t baudrate);
void initUart(uint8_t serialPort,uint8_t baudrate);
void terminateUart(uint8_t serialPort);
void writeOnUart0(uint8_t data);
void writeOnUart1(uint8_t data);
void writeOnUart2(uint8_t data);
int16_t  readFromUart0();
int16_t  readFromUart1();
int16_t  readFromUart2();
int16_t getAvailableDataCountOnUart0();
int16_t getAvailableDataCountOnUart1();
int16_t getAvailableDataCountOnUart2();


uint8_t digitalPinToUARTPort(uint8_t pin);

/**
 * @brief Setup the LEDs as UART communication indicator  
 * @param None
 * @return  None  
 */
void setupUartLeds();
/**
 * @brief Returns true if Arduino buffer is Empty.
 * @param None.  
 * @return True or False.  
 */
uint8_t getIsArduinoRx0BufferEmptyFlag();
/**
 * @brief Sets the boolean.
 * @param True or False.  
 * @return None.  
 */
void setIsArduinoRx0BufferEmptyFlag(uint8_t);
/**
 * @brief Returns true if Arduino buffer is Empty.
 * @param None.  
 * @return True or False.  
 */
uint8_t getIsArduinoRx0BufferOverFlowedFlag();
/**
 * @brief Sets the boolean.
 * @param True or False.  
 * @return None.  
 */
void setIsArduinoRx0BufferOverFlowedFlag(uint8_t);
#endif /* UART_H_ */