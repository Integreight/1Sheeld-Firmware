/*
 * mapping.h
 *
 * Created: 05/01/2014 06:17:13 م
 *  Author: HP
 */ 


#ifndef MAPPING_H_
#define MAPPING_H_



#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "CommonMacros.h"
#include "gpio.h"



#define TOTAL_PINS              35
#define digitalPinHasPWM(p)         ( ((p) == 3) || ((p) == 5) || ((p) == 6) ||  (p) == 9||  (p) == 10||  (p) == 11)
#define IS_PIN_DIGITAL(p)       ((p) >= 2 && (p) <= 22) || ((p)>=25 && (p)<=34)
#define IS_PIN_PWM(p)           digitalPinHasPWM(p)
#define IS_PIN_SPI(p)           ((p) == SS || (p) == MOSI || (p) == MISO || (p) == SCK)
#define PIN_TO_DIGITAL(p)       (p)
#define PIN_TO_PWM(p)           PIN_TO_DIGITAL(p)
#define NUM_ANALOG_INPUTS           0
#define analogInputToDigitalPin(p)  (-1)




#define true 0x1
#define false 0x0


#define NOT_A_PIN  255
#define NOT_A_PORT 255
// SPI pins 
static const uint8_t SS   = 10;
static const uint8_t MOSI = 28;
static const uint8_t MISO = 29;
static const uint8_t SCK  = 30;

//ATMEGA 162
//                  +-\/-+
// PWM  (D 9) PB0  1|    |40  VCC
// PWM  (D11) PB1  2|    |39  PA0 (D14)
//      (D23) PB2  3|    |38  PA1 (D15)
//      (D24) PB3  4|    |37  PA2 (D16)
// PWM  (D10) PB4  5|    |36  PA3 (D17)
//      (D28) PB5  6|    |35  PA4 (D18)
//      (D29) PB6  7|    |34  PA5 (D19)
//      (D30) PB7  8|    |33  PA6 (D20)
//            RST  9|    |32  PA7 (D21)
//      (D 0) PD0 10|    |31  PE0 (D33)
//      (D 1) PD1 11|    |30  PE1 (D34)
//      (D25) PD2 12|    |29  PE2 (D 6) PWM
//      (D26) PD3 13|    |28  PC7 (D32)
// PWM  (D 3) PD4 14|    |27  PC6 (D31)
// PWM  (D 5) PD5 15|    |26  PC5 (D13)
//      (D22) PD6 16|    |25  PC4 (D12)
//      (D27) PD7 17|    |24  PC3 (D 8)
//            XT2 18|    |23  PC2 (D 7)
//            XT1 19|    |22  PC1 (D 4)
//            GND 20|    |21  PC0 (D 2)
//                  +----+
//

/*
// pin numbers 
const uint16_t PROGMEM port_to_mode_PGM[] = {
	(uint16_t) &DDRA,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
	(uint16_t) &DDRE,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
    (uint16_t) &PORTA,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
	(uint16_t) &PORTE,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
    (uint16_t) &PINA,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
	(uint16_t) &PINE,
};
*/

const uint16_t PROGMEM port_to_register_PGM[] = {
	(uint16_t) 0x39,
	(uint16_t) 0x36,
	(uint16_t) 0x33,
	(uint16_t) 0x30,
	(uint16_t) 0x25,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PD, // PD0
	PD, // PD1 
	PC, // PC0
	PD, // PD4
	PC, // PC1
	PD, // PD5
	PE, // PE2
	PC, // PC2
	PC, // PC3
	PB, // PB0
	PB, // PB4
	PB, // PB1
	PC, // PC4
	PC, // PC5
	PA, // PA0
	PA, // PA1
	PA, // PA2
	PA, // PA3
	PA, // PA4
	PA, // PA5
	PA, // PA6
	PA, // PA7
	PD, // PD6
	PB, // PB2
	PB, // PB3
	PD, // PD2
	PD, // PD3
	PD, // PD7
	PB, // PB5
	PB, // PB6
	PB, // PB7
	PC, // PC6
	PC, // PC7
	PE, // PE0
	PE, // PE1
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	0,//_BV(0), 
	1,//_BV(1),
	0,//_BV(0),
	4,//_BV(4),
	1,//_BV(1),
	5,//_BV(5),
	2,//_BV(2), // PORT 
	2,//_BV(2),
	3,//_BV(3), /* 8, port B */
	0,//_BV(0),
	4,//_BV(4),
	1,//_BV(1),
	4,//_BV(4),
	5,//_BV(5),
	0,//_BV(0), /* 14, port A */
	1,//_BV(1),
	2,//_BV(2),
	3,//_BV(3),
	4,//_BV(4),
	5,//_BV(5),
	6,//_BV(6),
	7,//_BV(7),
    6,//_BV(6),
    2,//_BV(2),
    3,//_BV(3),
    2,//_BV(2),
    3,//_BV(3),
    7,//_BV(7),
    5,//_BV(5),
    6,//_BV(6),
    7,//_BV(7),
    6,//_BV(6),
    7,//_BV(7),
    0,//_BV(0),
    1,//_BV(1),

};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER, /* 0 - port D */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER3A,
	NOT_ON_TIMER,
	TIMER1A,
	TIMER1B,
	NOT_ON_TIMER,
	NOT_ON_TIMER, 
	TIMER0,
	TIMER3B,
	TIMER2,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER, 
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER, 
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER, 
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER, 
	NOT_ON_TIMER,
};
/*
static inline unsigned char readPort(byte, byte) __attribute__((always_inline, unused));
static inline unsigned char readPort(byte port, byte bitmask)
{

	unsigned char out=0, pin=port*8;

	if (IS_PIN_DIGITAL(pin+0) && (bitmask & 0x01) && digitalRead(PIN_TO_DIGITAL(pin+0))) out |= 0x01;
	if (IS_PIN_DIGITAL(pin+1) && (bitmask & 0x02) && digitalRead(PIN_TO_DIGITAL(pin+1))) out |= 0x02;
	if (IS_PIN_DIGITAL(pin+2) && (bitmask & 0x04) && digitalRead(PIN_TO_DIGITAL(pin+2))) out |= 0x04;
	if (IS_PIN_DIGITAL(pin+3) && (bitmask & 0x08) && digitalRead(PIN_TO_DIGITAL(pin+3))) out |= 0x08;
	if (IS_PIN_DIGITAL(pin+4) && (bitmask & 0x10) && digitalRead(PIN_TO_DIGITAL(pin+4))) out |= 0x10;
	if (IS_PIN_DIGITAL(pin+5) && (bitmask & 0x20) && digitalRead(PIN_TO_DIGITAL(pin+5))) out |= 0x20;
	if (IS_PIN_DIGITAL(pin+6) && (bitmask & 0x40) && digitalRead(PIN_TO_DIGITAL(pin+6))) out |= 0x40;
	if (IS_PIN_DIGITAL(pin+7) && (bitmask & 0x80) && digitalRead(PIN_TO_DIGITAL(pin+7))) out |= 0x80;
	

	return out;
}

static inline unsigned char writePort(byte, byte, byte) __attribute__((always_inline, unused));
static inline unsigned char writePort(byte port, byte value, byte bitmask)
{
   
	byte pin=port*8;
	if ((bitmask & 0x01)) digitalWrite(PIN_TO_DIGITAL(pin+0), (value & 0x01));
	if ((bitmask & 0x02)) digitalWrite(PIN_TO_DIGITAL(pin+1), (value & 0x02));
	if ((bitmask & 0x04)) digitalWrite(PIN_TO_DIGITAL(pin+2), (value & 0x04));
	if ((bitmask & 0x08)) digitalWrite(PIN_TO_DIGITAL(pin+3), (value & 0x08));
	if ((bitmask & 0x10)) digitalWrite(PIN_TO_DIGITAL(pin+4), (value & 0x10));
	if ((bitmask & 0x20)) digitalWrite(PIN_TO_DIGITAL(pin+5), (value & 0x20));
	if ((bitmask & 0x40)) digitalWrite(PIN_TO_DIGITAL(pin+6), (value & 0x40));
	if ((bitmask & 0x80)) digitalWrite(PIN_TO_DIGITAL(pin+7), (value & 0x80));
}

*/
#define digitalPinToPort(P) ( pgm_read_byte( digital_pin_to_port_PGM + (P) ) )
#define digitalPinToBitMask(P) ( pgm_read_byte( digital_pin_to_bit_mask_PGM + (P) ) )
#define digitalPinToTimer(P) ( pgm_read_byte( digital_pin_to_timer_PGM + (P) ) )
#define analogInPinToBit(P) (P)
//#define portOutputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_output_PGM + (P))) )
//#define portInputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_input_PGM + (P))) )
//#define portModeRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_mode_PGM + (P))) )
#define portModeRegister(P) ( ( pgm_read_word( port_to_register_PGM + (P))) )

#endif /* MAPPING_H_ */