#include "main.h"
#include "sw_uart.h"
#include "eeprom.h"

void main(){
    swuart_init();
    DDRB |= (1 << DD3) | (1 << DD2);
    //EEPROM_write(0, 0x42);
    swuart_transmit(0x55);
    //swuart_transmit(0x56);
    while(swuart_isTransmitterBusy());
    // swuart_transmit(0x57);
    // swuart_transmit(0x58);
    // swuart_transmit(0x59);
    // swuart_transmit(0x60);
    // swuart_transmit(0x61);
    swuart_receive();
    while(swuart_availableByte() == 0 );
    swuart_transmit(swuart_getReceivedByte());

     while(1){
    //     PORTB |= (1 << PB3);
    //     _delay_ms(500);
    //     PORTB &= ~(1 << PB3); 
    //     _delay_ms(500);
     }
}
