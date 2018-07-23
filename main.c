#include "main.h"
#include <util/delay.h>
#include "sw_uart.h"
#include "cfg_loader.h"
#include "eeprom.h"

void main(){
    swuart_init();
    DDRB |= (1 << PB3) | (1 << PB0);
    //EEPROM_write(0, 0x42);
    swuart_transmit(0x55);
    swuart_transmit(0x56);
    swuart_transmit(0x57);
    swuart_transmit(0x58);
    swuart_transmit(0x59);
    swuart_transmit(0x60);
    swuart_transmit(0x61);
    //swuart_transmit(cfg_loader_getDeviceAddress());
    //swuart_transmit(cfg_loader_getTCNT_val());
    //swuart_receive();
    //while(rx_buffer_index_head < 1){}
    //swuart_transmit(rx_buffer[0]);

    

     while(1){
    //     PORTB |= (1 << PB3);
    //     _delay_ms(500);
    //     PORTB &= ~(1 << PB3); 
    //     _delay_ms(500);
     }
}
