#ifndef INC_PACKET_H
#define INC_PACKET_H

#include <inttypes.h>

#define PACKET_MASTER_ADDRESS 0x11
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


uint8_t packet_validate(struct packet_t *p);
uint8_t packet_genFrameCheck(struct packet_t *p);
void packet_send(struct packet_t *p);

#endif