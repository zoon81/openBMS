#ifndef INC_TWI_H
#define INC_TWI_H

#include <stdio.h>
#include "nrf_drv_twi.h"
#include "app_util_platform.h"
#include "ble400.h"

void twi_init (void);
void twi_tx(uint8_t address, uint8_t const *p_data,uint32_t length,bool xfer_pending);

#endif


