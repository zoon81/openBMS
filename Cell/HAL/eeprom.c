#include "eeprom.h"

void EEPROM_write(uint8_t address, uint8_t data){
    /* Wait for completion of previous write */
    while (EECR & (1 << EEPE));
    /* Set Programming mode */
    EECR = 0;
    /* Set up address and data registers */
    EEARL = address;
    EEDR = data;
    /* Write logical one to EEMPE */
    EECR |= (1 << EEMPE);
    /* Start eeprom write by setting EEPE */
    EECR |= (1 << EEPE);
}

uint8_t EEPROM_read(unsigned char address)
{
    /* Wait for completion of previous write */
    while (EECR & (1 << EEPE));
    /* Set up address register */
    EEARL = address;
    /* Start eeprom read by writing EERE */
    EECR |= (1 << EERE);
    /* Return data from data register */
    return EEDR;
}