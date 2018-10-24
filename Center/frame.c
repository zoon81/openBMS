#include "frame.h"
#define CRC_POLY 0x91

struct packetbuff_t tmp_packet;
struct packet_t response_packet;

static uint8_t packet_validate(struct packet_t *p);
static uint8_t packet_genFrameCheck(struct packet_t *p);
static void packet_send(struct packet_t *p);
static uint8_t crc8_update(uint8_t crc, uint8_t data);

void frame_init()
{
    uart_init();
    memset(&tmp_packet, 0, sizeof(tmp_packet));
    memset(&response_packet, 0, sizeof(response_packet));
}

void uart_InCommingData_handler(uint8_t data)
{
    if ((data == PACKET_MASTER_ADDR) && (tmp_packet.address == 0)){
        tmp_packet.address = data;
        tmp_packet.index++;
    }
    else if ((tmp_packet.address == PACKET_MASTER_ADDR) && (tmp_packet.index < 5)){
        if (tmp_packet.index == 2){
            //Upper byte of data field
            ((uint8_t *)(&tmp_packet))[3] = data;
            tmp_packet.index++;
        }
        else if (tmp_packet.index == 3){
            //Lower byte of data field
            ((uint8_t *)(&tmp_packet))[2] = data;
            tmp_packet.index++;
        } else {
            ((uint8_t *)(&tmp_packet))[tmp_packet.index] = data;
            tmp_packet.index++;
        }
    }
    // Receiving packet DONE, validate and copy to the final buffer
    if (tmp_packet.index > 4){
        if (packet_validate(&tmp_packet)){
            response_packet.address = tmp_packet.address;
            response_packet.command = tmp_packet.command;
            response_packet.data = tmp_packet.data;
            response_packet.crc = tmp_packet.crc;
            memset(&tmp_packet, 0, sizeof(tmp_packet));
        }
    }
}

uint8_t packet_isResponseAvailable()
{
    return response_packet.address == PACKET_MASTER_ADDR ? 1 : 0;
}
// Get the response of the CellVoltage request
uint8_t cell_getVoltage(uint16_t *Vbat)
{
    if (packet_isResponseAvailable())
    {
        if (response_packet.command == PACKET_CMD_BAT_V)
        {
            *Vbat = response_packet.data;
            memset(&response_packet, 0, sizeof(response_packet));
            return 1;
        }
    }
    return 0;
}

uint16_t packet_requestCellVoltage(uint8_t celladdress)
{
    struct packet_t pkg;
    pkg.address = celladdress;
    pkg.command = PACKET_CMD_BAT_V;
    pkg.data = 0;
    pkg.crc = packet_genFrameCheck(&pkg);
    packet_send(&pkg);
}

static uint8_t packet_validate(struct packet_t *p)
{
    return packet_genFrameCheck(&(*p)) == p->crc ? 1 : 0;
}

static uint8_t packet_genFrameCheck(struct packet_t *p)
{
    uint8_t crc = 0;
    crc = crc8_update(crc, p->address);
    crc = crc8_update(crc, p->command);
    crc = crc8_update(crc, (uint8_t)((p->data) >> 8));
    crc = crc8_update(crc, (uint8_t)((p->data) & 0xFF));
    return crc;
}

static void packet_send(struct packet_t *p)
{
    app_uart_put(p->address);
    app_uart_put(p->command);
    app_uart_put((uint8_t)((p->data) >> 8));
    app_uart_put((uint8_t)((p->data) & 0xFF));
    app_uart_put(p->crc);
}

static uint8_t crc8_update(uint8_t crc, uint8_t data)
{
    crc ^= data;
    for (uint8_t i = 8; i; --i)
    {
        if (crc & 1)
            crc ^= CRC_POLY;
        crc >>= 1;
    }
    return crc;
}