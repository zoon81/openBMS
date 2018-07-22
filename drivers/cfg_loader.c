#include "cfg_loader.h"

uint8_t cfg_loader_getDeviceAddress(){
    return EEPROM_read(CFGLD_DEVICE_ADDR);
}

int8_t cfg_loader_getTCNT_val(){
    return EEPROM_read(CFGLD_TCNT_CAL);
}

uint8_t cfg_loader_getBandGap_val(){
    return EEPROM_read(CFGLD_BANDGAP_CAL);
}