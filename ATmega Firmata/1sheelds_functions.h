/*

  Project:       1Sheeld Firmware 
  File:          1sheelds_functions.h

  Compiler:      Arduino avr-gcc 4.3.2

  Author:        Integreight
                 
  Date:          2014.5

*/
#include "sys.h"
#include "gpio.h"
#include "pwm.h"
#include "uart.h"
#include "mapping162.h"
#include <util/delay.h>

#ifndef SHEELDS_FUNCTIONS_H_
#define SHEELDS_FUNCTIONS_H_



uint8  digitalRead(uint8);
void   digitalWrite(uint8, uint8);
void   pinMode(uint8 , uint8);
void   analogWrite(uint8, int);
unsigned long pulseIn(uint8_t pin, uint8_t state);
unsigned long readPWM (int pin);
void   writePort(byte, byte, byte);
uint8  readPort(byte, byte);
int serial0_Avilable();
int serial1_Avilable();
void turnOffPWM(uint8 timer);
void unusedPinsAsOutput();
void resetBluetooth();

#endif /* 1SHEELDS_FUNCTIONS_H_ */