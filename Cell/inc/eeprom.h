#ifndef INC_EEPROM_H
#define INC_EEPROM_H

#include <avr/io.h>

void EEPROM_write(uint8_t, uint8_t );
uint8_t EEPROM_read(uint8_t);

#endif