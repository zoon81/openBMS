#ifndef INC_CFG_LOADER
#define INC_CFG_LOADER
#include "eeprom.h"
//define eeprom data structure

#define CFGLD_DEVICE_ADDR 0
#define CFGLD_TCNT_CAL 1
#define CFGLD_BANDGAP_CAL 2

uint8_t cfg_loader_getDeviceAddress();
int8_t cfg_loader_getTCNT_val();
uint8_t cfg_loader_getBandGap_val();

#endif