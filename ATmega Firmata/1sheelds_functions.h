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
void   writePort(byte, byte, byte);
uint8  readPort(byte, byte);
int serial0_Avilable();
int serial1_Avilable();
void turnOffPWM(uint8 timer);



#endif /* 1SHEELDS_FUNCTIONS_H_ */