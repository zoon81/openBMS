#include "packet.h"
#include "util.h"
#include "sw_uart.h"

<<<<<<< HEAD
#define CRC_POLY 0x91

uint8_t crc8_update(uint8_t crc, uint8_t data);

=======
>>>>>>> 0af11edb2c3301e8d9ec7259e5c7c1e7211edd48
uint8_t packet_validate(struct packet_t *p){
   return packet_genFrameCheck( &(*p) ) == p->crc ? 1 : 0;
}

uint8_t packet_genFrameCheck(struct packet_t *p){
    uint8_t crc = 0;
    crc = crc8_update(crc, p->address);
    crc = crc8_update(crc, p->command);
<<<<<<< HEAD
    crc = crc8_update(crc, (uint8_t) ((p->data) >>  8));
    crc = crc8_update(crc, (uint8_t) ((p->data) & 0xFF));
=======
    crc = crc8_update(crc, p->data);
>>>>>>> 0af11edb2c3301e8d9ec7259e5c7c1e7211edd48
    return crc;
}

void packet_send(struct packet_t *p){
    swuart_transmit(p->address);
    swuart_transmit(p->command);
<<<<<<< HEAD
    swuart_transmit( (uint8_t) ((p->data) >>  8));
    swuart_transmit( (uint8_t) ((p->data) & 0xFF));
    swuart_transmit(p->crc);
}

uint8_t crc8_update(uint8_t crc, uint8_t data){
	crc ^= data;
	for (uint8_t i = 8; i; --i) {
		if (crc & 1)
			crc ^= CRC_POLY;
		crc >>= 1;
	}
	return crc;
=======
    swuart_transmit(p->data);
    swuart_transmit(p->crc);
>>>>>>> 0af11edb2c3301e8d9ec7259e5c7c1e7211edd48
}