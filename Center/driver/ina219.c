#include "ina219.h"
uint16_t cfg_reg_value;

void ina219_init(){
    twi_init();
    ina219_updateConfig(INA_CONFIG_REG_INIT_VALUE);    
}

void ina219_updateConfig(uint16_t cfg){
    cfg_reg_value = cfg;
    twi_tx(INA219_ADDRESS_W, &cfg_reg_value, 2, false);
}
