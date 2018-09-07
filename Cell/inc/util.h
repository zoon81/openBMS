#ifndef INC_UTIL_H
#define INC_UTIL_H

#include <inttypes.h>
#include "sw_uart.h"

#define UART_SENDBYTE(x) swuart_transmit((x))


uint8_t crc8_update(uint8_t crc, uint8_t data);
void uint8_to_str(char *buffer, uint8_t number);
void uint16_to_str(char *buffer, uint32_t number);
void int16_to_str(char *buffer, int16_t number);
<<<<<<< HEAD
=======
uint16_t atoi_(char *str);
uint8_t strlen_(char *str);
uint16_t pwr(uint8_t base, uint8_t exponent);
>>>>>>> 0af11edb2c3301e8d9ec7259e5c7c1e7211edd48
void puts_uart(char *str);


#endif