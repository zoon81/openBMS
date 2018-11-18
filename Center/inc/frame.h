#ifndef INC_FRAME_H
#define INC_FRAME_H

#include <string.h>
#include "uart.h"
#include "systick.h"
#define PACKET_MASTER_ADDR      0x11

#define PACKET_CMD_BAT_V        0x33
#define PACKET_CMD_BAT_V_FAST   0x35
#define PACKET_CMD_TEMP         0xC6
#define PACKET_CMD_BALLANCE     0x55
#define PACKET_CMD_PING         0xE9
#define PACKET_CMD_STATUS       0xE5

// Cell status
#define STATUS_IDLE                 0
#define STATUS_SLEEPP               1
#define STATUS_BALLANC_INPROGGRESS  2
#define STATUS_BALLANC_DONE         4

#define CELL1_ADDR 0
#define CELL2_ADDR 1
#define CELL3_ADDR 2

// Uart communication GPIO def, each cell has a unique interface
#define UART_PIN_TX1    1
#define UART_PIN_RX1    2
#define UART_PIN_TX2    3
#define UART_PIN_RX2    4
#define UART_PIN_TX3    5
#define UART_PIN_RX4    6

#define RX_PIN_BY_ADDRESS(ADDRESS) (((ADDRESS) * 2) + 2)
#define TX_PIN_BY_ADDRESS(ADDRESS) (((ADDRESS) * 2) + 1)

#define PACKET_TIMEOUT 5
#define PACKET_SIZE 5
#define PACKET_PAYLOAD_SIZE (PACKET_SIZE - 1)

struct packet_t{
    uint8_t address;
    uint8_t command;
    uint16_t data;
    uint8_t crc;
    uint8_t channel;
};

struct packetbuff_t{
    uint8_t address;
    uint8_t command;
    uint16_t data;
    uint8_t crc;
    uint8_t index;
    uint32_t timestamp;
};

void frame_init();
uint16_t cell_requestCellVoltage(uint8_t celladdress);
uint8_t cell_getVoltage(uint16_t *Vbat);

#endif