#ifndef INC_FRAME_H
#define INC_FRAME_H

#include <string.h>
#include "uart.h"
#define PACKET_MASTER_ADDR      0x11
#define PACKET_CMD_BAT_V        0x33
#define PACKET_CMD_BAT_V_FAST   0x35
#define PACKET_CMD_TEMP         0xC6
#define PACKET_CMD_BALLANCE     0x55
#define PACKET_CMD_PING         0xE9

#define PACKET_TIMEOUT 5
#define PACKET_SIZE 5
#define PACKET_PAYLOAD_SIZE (PACKET_SIZE - 1)

struct packet_t{
    uint8_t address;
    uint8_t command;
    uint16_t data;
    uint8_t crc;
};

struct packetbuff_t{
    uint8_t address;
    uint8_t command;
    uint16_t data;
    uint8_t crc;
    uint8_t index;
};

void frame_init();
uint16_t packet_requestCellVoltage(uint8_t celladdress);
uint8_t cell_getVoltage(uint16_t *Vbat);

#endif