#ifndef INC_TWI_H
#define INC_TWI_H

#include <stdio.h>
#include "nrf_drv_twi.h"
#include "app_util_platform.h"
#include "ble400.h"

#define TWI_SDA_PIN 0
#define TWI_SCL_PIN 1

#define TWI_STATUS_IDLE 0x00
#define TWI_STATUS_SENDING 0x01
#define TWI_STATUS_RECEIVING 0x02
#define TWI_STATUS_BUSY 0x03

void twi_init (void);

void twi_setTxDoneHandler(void (*txDoneHandler)());
void twi_setRxDoneHandler(void (*txDoneHandler)());
ret_code_t twi_tx(uint8_t address, uint8_t const *p_data,uint32_t length,bool xfer_pending);
ret_code_t twi_rx(uint8_t address, uint8_t *incommingdata, uint8_t length, bool xref_pending);
uint8_t twi_getState();
void twi_setState(uint8_t state);
#endif


