
#ifndef MAPPING_H_
#define MAPPING_H_
#include "stdint.h"
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "macros.h"



#define digitalPinHasPWM(p)         ( ((p) == 3) || ((p) == 5) || ((p) == 6) ||  ((p) == 9) ||  ((p) == 10) ||  ((p) == 11) )
#define IS_PIN_DIGITAL(p)       ((p) >= 2 && (p) <= 19)
#define IS_PIN_PWM(p)           digitalPinHasPWM(p)
#define IS_PIN_SPI(p)           ((p) == SS || (p) == MOSI || (p) == MISO || (p) == SCK)
#define NUM_ANALOG_INPUTS           0
#define analogInputToDigitalPin(p)  (-1)

#define NOT_A_PIN 255
#define NOT_A_PORT 255
// SPI pins 
static const uint8_t SS   = 10;
static const uint8_t MOSI = 28;
static const uint8_t MISO = 29;
static const uint8_t SCK  = 30;



extern const uint16_t PROGMEM port_to_input_PGM[];

extern const uint16_t PROGMEM port_to_register_PGM[];

extern const uint8_t PROGMEM digital_pin_to_port_PGM[];

extern const uint8_t PROGMEM digital_pin_to_bit_mask_PWM_PGM[];

extern const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[];

extern const uint8_t PROGMEM digital_pin_to_timer_PGM[];

#define digitalPinToPort(P) ( pgm_read_byte( digital_pin_to_port_PGM + (P) ) )
#define digitalPinToBitMask(P) ( pgm_read_byte( digital_pin_to_bit_mask_PGM + (P) ) )
#define digitalPinToTimer(P) ( pgm_read_byte( digital_pin_to_timer_PGM + (P) ) )
#define analogInPinToBit(P) (P)
#define portModeRegister(P) ( ( pgm_read_word( port_to_register_PGM + (P))) )

#define digitalPinToBitMaskPWM(P) ( pgm_read_byte( digital_pin_to_bit_mask_PWM_PGM + (P) ) )
// for pulseIn
#define clockCyclesPerMicrosecond() ( F_CPU /(double) 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )
#define portInputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_input_PGM + (P))) )
#endif /* MAPPING_H_ */