#include "packet.h"
#include "util.h"
#include "sw_uart.h"

uint8_t packet_validate(struct packet_t *p){
   return packet_genFrameCheck( &(*p) ) == p->crc ? 1 : 0;
}

uint8_t packet_genFrameCheck(struct packet_t *p){
    uint8_t crc = 0;
    crc = crc8_update(crc, p->address);
    crc = crc8_update(crc, p->command);
    crc = crc8_update(crc, p->data);
    return crc;
}

void packet_send(struct packet_t *p){
    swuart_transmit(p->address);
    swuart_transmit(p->command);
    swuart_transmit(p->data);
    swuart_transmit(p->crc);
}