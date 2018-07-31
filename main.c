#include "main.h"
#include "sw_uart.h"
#include "eeprom.h"
#include "packet.h"

void main(){
    swuart_init();
    DDRB |= (1 << DD3);
    swuart_transmit(0x55);
    while(swuart_isTransmitterBusy());
    
    while(1){
        swuart_receive();
        while(swuart_availableByte() == 0 );
        uint8_t data = swuart_getReceivedByte();
        
        switch (data)
        {
            case PACKET_CMD_PING:
                //swuart_transmit(DEVICE_ADDRESS);
                break;

            default:
                break;
        }
        swuart_transmit(data);
        while(swuart_isTransmitterBusy());
    }
}
