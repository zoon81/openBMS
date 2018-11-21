#ifndef INC_EEPROM_API_H
#define INC_EEPROM_API_H

#include <avr/io.h>

void EEPROM_write(uint8_t address, uint8_t data);
uint8_t EEPROM_read(uint8_t address);

#endif