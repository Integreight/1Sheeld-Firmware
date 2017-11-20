#include "config.h"				   
#include "mapping.h"

const uint16_t PROGMEM port_to_input_PGM[] = {
	(uint16_t) &PINA,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
	(uint16_t) &PINE,
};

const uint16_t PROGMEM port_to_register_PGM[] = {
	(uint16_t) 0x20,
	(uint16_t) 0x23,
	(uint16_t) 0x26,
	(uint16_t) 0x29,
	(uint16_t) 0x2C,
};

 const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	 PD, // PD0
	 PD, // PD1
	 PE, // PE2
	 PD, // PD5
	 PE, // PE3
	 PD, // PD6
	 PD, // PD4
	 PC, // PC0
	 PC, // PC1
	 PB, // PB4
	 PB, // PB3
	 PD, // PD7
	 PC, // PC2
	 PC, // PC3
	 PA, // PA7
	 PA, // PA6
	 PA, // PA5
	 PA, // PA4
	 PA, // PA3
	 PA, // PA2
	 PC, // PC7
	 PC, // PC6
	 PE, // PE5
	 PD, // PD2
	 PD, // PD3
	 PA, // PA1
	 PA, // PA0
	 PE, // PE6
	 PB, // PB5
	 PB, // PB6
	 PB, // PB7
	 PB, // PB0
	 PB, // PB1
	 PC, // PC5
	 PB, // PB2
 };
 
 const uint8_t PROGMEM digital_pin_to_bit_mask_PWM_PGM[] = {
	 _BV(0),
	 _BV(1),
	 _BV(2),
	 _BV(5),
	 _BV(3),
	 _BV(6),
	 _BV(4), // PORT
	 _BV(0),
	 _BV(1), /* 8, port B */
	 _BV(4),
	 _BV(3),
	 _BV(7),
	 _BV(2),
	 _BV(3),
	 _BV(7), /* 14, port A */
	 _BV(6),
	 _BV(5),
	 _BV(4),
	 _BV(3),
	 _BV(2),
	 _BV(7),
	 _BV(6),
	 _BV(5),
	 _BV(2),
	 _BV(3),
	 _BV(1),
	 _BV(0),
	 _BV(6),
	 _BV(5),
	 _BV(6),
	 _BV(7),
	 _BV(0),
	 _BV(1),
	 _BV(5),
	 _BV(2),

 };

 const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	 0,//_BV(0),
	 1,//_BV(1),
	 2,//_BV(2),
	 5,//_BV(5),
	 3,//_BV(3),
	 6,//_BV(6),
	 4,//_BV(4), // PORT
	 0,//_BV(0),
	 1,//_BV(1), /* 8, port B */
	 4,//_BV(4),
	 3,//_BV(3),
	 7,//_BV(7),
	 2,//_BV(2),
	 3,//_BV(3),
	 7,//_BV(7), /* 14, port A */
	 6,//_BV(6),
	 5,//_BV(5),
	 4,//_BV(4),
	 3,//_BV(3),
	 2,//_BV(2),
	 7,//_BV(7),
	 6,//_BV(6),
	 5,//_BV(5),
	 2,//_BV(2),
	 3,//_BV(3),
	 1,//_BV(1),
	 0,//_BV(0),
	 6,//_BV(6),
	 5,//_BV(5),
	 6,//_BV(6),
	 7,//_BV(7),
	 0,//_BV(0),
	 1,//_BV(1),
	 5,//_BV(5),
	 2,//_BV(2),

 };
 
 const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	 NOT_ON_TIMER, /* 0 - port D */
	 NOT_ON_TIMER,
	 NOT_ON_TIMER,
	 TIMER_1A,
	 NOT_ON_TIMER,
	 TIMER_2B,
	 TIMER_1B,
	 NOT_ON_TIMER,
	 NOT_ON_TIMER,
	 TIMER_0B,
	 TIMER_0A,
	 TIMER_2A,
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
