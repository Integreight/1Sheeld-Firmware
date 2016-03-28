/*

  Project:       1Sheeld Firmware 
  File:          eeprom.h

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
                 
  Date:          2016.4

*/

#ifndef EEPROM_H_
#define EEPROM_H_

#include <avr/eeprom.h>


uint8_t readFromEeprom(uint8_t);
void writeToEeprom(uint8_t , uint8_t);
void updateEeprom(uint8_t , uint8_t);


#endif /* EEPROM_H_ */