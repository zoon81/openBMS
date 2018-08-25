#ifndef INC_FIFO_H
#define INC_FIFO_H
#include <avr/io.h>
#include <inttypes.h>

struct fifo_t {
    uint8_t *buffer;
    uint8_t used;
    uint8_t size;
    uint8_t head;
    uint8_t tail;
};

uint8_t fifo_push(struct fifo_t *fifo, uint8_t data);
uint8_t fifo_pull(struct fifo_t *fifo, uint8_t *buffer);
void fifo_steptail(struct fifo_t *fifo);
#endif