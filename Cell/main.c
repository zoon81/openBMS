#include "main.h"
#include "util/delay.h"
#include "sw_uart.h"
#include "eeprom.h"
#include "packet.h"
#include "adc.h"
#include "util.h"

void main(){
    swuart_init();
    adc_init();
    DDRB |= (1 << DD0) | (1 << DD1);
    swuart_transmit(0x55);
    while(swuart_isTransmitterBusy());
    
    while(1){
        uint8_t msec_counter = 0;
        struct packet_t p;
        swuart_receive();
        while(swuart_availableByte() == 0 );
        uint8_t dev_addr = swuart_getReceivedByte();
        //This packet is for me?
        if (dev_addr == DEVICE_ADDRESS){
            //Wait until we get the packet or we get timeout
            while (msec_counter < PACKET_TIMEOUT && swuart_availableByte() < PACKET_PAYLOAD_SIZE){
                _delay_ms(1);
                msec_counter++;
            }
            if (msec_counter < PACKET_TIMEOUT)
            {
                //Parsing the packet
                p.address = dev_addr;
                p.command = swuart_getReceivedByte();
                p.data = swuart_getReceivedByte();
                p.crc = swuart_getReceivedByte();
                if ( packet_validate(&p) ){
                    switch (p.command)
                    {
                    case PACKET_CMD_PING:
                        swuart_transmit(DEVICE_ADDRESS);
                        break;
                    case PACKET_CMD_BAT_V:;
                        uint16_t bat_v = adc_getVcc();
                        struct packet_t resp;
                        resp.address = PACKET_MASTER_ADDRESS;
                        resp.command = p.command;
                        resp.data = bat_v;
                        resp.crc = packet_genFrameCheck(&resp);
                        packet_send(&resp);
                        break;
                    case PACKET_CMD_BALLANCE:
                        break;
                    default:
                        break;
                    }
                }
                while (swuart_isTransmitterBusy())
                    ;
            }
            // Packet timeout
            else{

            }
        } 
        // This packet is not for me
        else {

        }
    } 
}
