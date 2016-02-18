/*

  Project:       1Sheeld Firmware 
  File:          atmega162_mapping.h

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
                 
  Date:          2014.5

*/

#ifndef ATMEGA162_MAPPING_H_
#define ATMEGA162_MAPPING_H_
#define F_CPU 7372800UL

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "CommonMacros.h"
#include "sys.h"

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

//ATMEGA 162
//                  +-\/-+
// PWM  (D11) PB0  1|    |40  VCC
// PWM  (D 9) PB1  2|    |39  PA0 (D19)
//      (D23) PB2  3|    |38  PA1 (D18)
//      (D24) PB3  4|    |37  PA2 (D17)
// PWM  (D10) PB4  5|    |36  PA3 (D16)
//      (D28) PB5  6|    |35  PA4 (D15)
//      (D29) PB6  7|    |34  PA5 (D14)
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