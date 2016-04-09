/*

  Project:       1Sheeld Firmware 
  File:          eeprom.cpp

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
                 
  Date:          2016.4

*/
#include "config.h"
#include "eeprom.h"



uint8_t readFromEeprom(uint8_t location)
{
	return eeprom_read_byte((uint8_t*)(uint16_t)location);
}

void writeToEeprom(uint8_t location, uint8_t data)
{
	eeprom_write_byte ((uint8_t*)(uint16_t)location,data);
}

void updateEeprom(uint8_t  location, uint8_t data)
{
	eeprom_update_byte((uint8_t*)(uint16_t)location,data);
}