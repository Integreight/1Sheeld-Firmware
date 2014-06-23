/*
 * mapping162.c
 *
 * Created: 17/6/2014 7:34:42 PM
 *  Author: iMustafa
 */ 
#include "mapping162.h"

#define F_CPU 7372800UL

const uint16_t PROGMEM port_to_input_PGM[] = {
	(uint16_t) &PINA,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
	(uint16_t) &PINE,
};

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
	 PB, // PB1
	 PB, // PB4
	 PB, // PB0
	 PC, // PC4
	 PC, // PC5
	 PA, // PA5
	 PA, // PA4
	 PA, // PA3
	 PA, // PA2
	 PA, // PA1
	 PA, // PA0
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
 
 const uint8_t PROGMEM digital_pin_to_bit_mask_PWM_PGM[] = {
	 _BV(0),
	 _BV(1),
	 _BV(0),
	 _BV(4),
	 _BV(1),
	 _BV(5),
	 _BV(2), // PORT
	 _BV(2),
	 _BV(3), /* 8, port B */
	 _BV(0),
	 _BV(4),
	 _BV(1),
	 _BV(4),
	 _BV(5),
	 _BV(5), /* 14, port A */
	 _BV(4),
	 _BV(3),
	 _BV(2),
	 _BV(1),
	 _BV(0),
	 _BV(6),
	 _BV(7),
	 _BV(6),
	 _BV(2),
	 _BV(3),
	 _BV(2),
	 _BV(3),
	 _BV(7),
	 _BV(5),
	 _BV(6),
	 _BV(7),
	 _BV(6),
	 _BV(7),
	 _BV(0),
	 _BV(1),

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
	 1,//_BV(0),
	 4,//_BV(4),
	 0,//_BV(1),
	 4,//_BV(4),
	 5,//_BV(5),
	 5,//_BV(0), /* 14, port A */
	 4,//_BV(1),
	 3,//_BV(2),
	 2,//_BV(3),
	 1,//_BV(4),
	 0,//_BV(5),
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
	 TIMER2,
	 TIMER3B,
	 TIMER0,
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
