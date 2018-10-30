#ifndef INC_UART_H
#define INC_UART_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_uart.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf.h"
#include "ble400.h"

#define UART_TX_BUF_SIZE 16                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 2                           /**< UART RX buffer size. */

uint32_t uart_init();
void uart_setIncommingDataHandler( void (*inCommingDataHandler)(uint8_t) );


#endif