#ifndef INC_PACKET_H
#define INC_PACKET_H

#include <inttypes.h>

#define PACKET_CMD_BAT_V        0x33
#define PACKET_CMD_BAT_V_FAST   0x35
#define PACKET_CMD_TEMP         0xC6
#define PACKET_CMD_BALLANCE     0x55
#define PACKET_CMD_PING         0xE9

struct packet_t{
    uint8_t address;
    uint8_t command;
    uint16_t data;
    uint8_t crc;
};

#endif