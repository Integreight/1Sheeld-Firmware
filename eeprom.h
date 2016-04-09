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

/**
 * @brief Read byte from a certain EEPROM cell.
 * @param Cell address to be extracting the byte from.
 * @return Data byte. 
 */
uint8_t readFromEeprom(uint8_t);
/**
 * @brief Write a byte to a certain EEPROM cell.
 * @param Data byte and the address to be written to.
 * @return None. 
 */
void writeToEeprom(uint8_t , uint8_t);
/**
 * @brief Checks the cell data before writing then updating it if the data is not equal.
 * @param Data byte and the address to be written to.
 * @return None. 
 */
void updateEeprom(uint8_t , uint8_t);


#endif /* EEPROM_H_ */