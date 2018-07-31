#include "util.h"

uint8_t crc8_update(uint8_t crc, uint8_t data)
{
	crc ^= data;
	for (uint8_t i = 8; i; --i) {
		if (crc&1)
			crc = (crc>>1) ^ 140;
		else
			crc >>= 1;
	}
	return crc;
}