#include "packet.h"
#include "util.h"
#include "sw_uart.h"

#define CRC_POLY 0x91

uint8_t crc8_update(uint8_t crc, uint8_t data);

uint8_t packet_validate(struct packet_t *p){
   return packet_genFrameCheck( &(*p) ) == p->crc ? 1 : 0;
}

uint8_t packet_genFrameCheck(struct packet_t *p){
    uint8_t crc = 0;
    crc = crc8_update(crc, p->address);
    crc = crc8_update(crc, p->command);
    crc = crc8_update(crc, (uint8_t) ((p->data) >>  8));
    crc = crc8_update(crc, (uint8_t) ((p->data) & 0xFF));
    return crc;
}

void packet_send(struct packet_t *p){
    swuart_transmit(p->address);
    swuart_transmit(p->command);
    swuart_transmit( (uint8_t) ((p->data) >>  8));
    swuart_transmit( (uint8_t) ((p->data) & 0xFF));
    swuart_transmit(p->crc);
}

uint8_t crc8_update(uint8_t crc, uint8_t data){
	crc ^= data;
    uint8_t i;
	for (i = 8; i; --i) {
		if (crc & 1)
			crc ^= CRC_POLY;
		crc >>= 1;
	}
	return crc;
}