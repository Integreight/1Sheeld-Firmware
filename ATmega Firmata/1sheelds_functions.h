/*
 * _1sheelds_functions.h
 *
 * Created: 24/12/2013 08:51:42 م
 *  Author: HP
 */ 


#ifndef SHEELDS_FUNCTIONS_H_
#define SHEELDS_FUNCTIONS_H_

#include "sys.h"

uint8  digitalRead(uint8);
void   digitalWrite(uint8, uint8);
void   pinMode(uint8 , uint8);
void   analogWrite(uint8, int);
unsigned long pulseIn(uint8_t pin, uint8_t state);
unsigned long readPWM (int pin);
void   writePort(byte, byte, byte);
uint8  readPort(byte, byte);
uint16_t serial0_Avilable();
int serial1_Avilable();
void turnOffPWM(uint8 timer);
void unusedPinsInput();


#endif /* 1SHEELDS_FUNCTIONS_H_ */