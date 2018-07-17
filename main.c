#include "main.h"
#include <util/delay.h>
#include "sw_uart.h"

void main(){
    swuart_init();
    DDRB |= (1 << PB3) | (1 << PB0);
    swuart_transmit(55);
    _delay_ms(5);
    swuart_receive();
    while(rx_buffer_index_head < 1){}
    swuart_transmit(rx_buffer[0]);

     while(1){
    //     PORTB |= (1 << PB3);
    //     _delay_ms(500);
    //     PORTB &= ~(1 << PB3); 
    //     _delay_ms(500);
     }
}
