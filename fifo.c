#include "fifo.h"

void _fifo_steptail(struct fifo_t **fifo);

uint8_t fifo_push(struct fifo_t *fifo, uint8_t data){
    if( fifo->used >= fifo->size ){
        return 1;
    } else {                           
        fifo->buffer[fifo->head] = data;
        fifo->head++;
        fifo->used++;
        return 0;       
    }
}

uint8_t fifo_pull(struct fifo_t *fifo, uint8_t *buffer){
    if(fifo->used > 0){
        *buffer = fifo->buffer[fifo->tail];
        _fifo_steptail(&fifo);
        return 0;
    } else {
        return 1;
    }
}

//handling tail and head togeather
void _fifo_steptail(struct fifo_t **fifo){
    (*fifo)->tail++;
    (*fifo)->used--;
    if((*fifo)->tail >= (*fifo)->size){
        (*fifo)->tail = 0;
    }
    if( (*fifo)->head >= (*fifo)->size){
        (*fifo)->head = 0;
    }
       
}

//helper for _fifo_steptail
void fifo_steptail(struct fifo_t *fifo){
    _fifo_steptail( &fifo);
}