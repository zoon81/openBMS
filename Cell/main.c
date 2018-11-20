#include "main.h"
#include "util/delay.h"
#include "sw_uart.h"
#include "eeprom.h"
#include "packet.h"
#include "adc.h"
#include "util.h"

void do_ballance();

uint8_t status = STATUS_IDLE;
uint16_t ballanceV = 0;
uint8_t systick_250ms_counter = 0;

void main(){
    
    swuart_init();
    
    // Ballancing task scheduler
    // Init TIMER1 in CTC mode, IRQ in every 250ms
    // Timer not stared here.
    OCR1C = 244;
    TIMSK |= (1 << TOIE1);
    TCCR1 |= (1 << CTC1) |(1 << PWM1A);
    
    while(1){
        uint8_t msec_counter = 0;
        struct packet_t p;
        // Activate INT0 interrupt, this will wake up the device
        swuart_receive();

        //Turn off adc to save power and go sleep
        adc_deinit();
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_mode();

        // We land here after wake up (INT0 goes low)
        
        // The first ADC conversion result after switching voltage reference source maybe inaccurate, and the user is advised to discard this result.
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
                //Parsing raw data
                p.address = dev_addr;
                p.command = swuart_getReceivedByte();
                p.data = swuart_getReceivedByte();
                p.data <<= 8;
                p.data |= swuart_getReceivedByte();
                p.crc = swuart_getReceivedByte();
                if ( packet_validate(&p) ){
                    switch (p.command){
                    case PACKET_CMD_PING:{
                        swuart_transmit(DEVICE_ADDRESS);
                        break;
                        }
                    case PACKET_CMD_BAT_V:;{
                        adc_init();
                        // delay for internal reference stabilization
                        _delay_ms(2);
                        adc_getVcc();
                        uint16_t bat_v = adc_getVcc();
                        struct packet_t resp;
                        resp.address = PACKET_MASTER_ADDRESS;
                        resp.command = p.command;
                        resp.data = bat_v;
                        resp.crc = packet_genFrameCheck(&resp);
                        packet_send(&resp);
                        break;
                        }
                    case PACKET_CMD_BALLANCE:{
                        ballanceV = p.data;
                        TCNT1 = 0;
                        // Start Timer1
                        do_ballance();
                        TCCR1 |= 3;
                        struct packet_t resp;
                        resp.address = PACKET_MASTER_ADDRESS;
                        resp.command = p.command;
                        resp.data = status;
                        resp.crc = packet_genFrameCheck(&resp);
                        packet_send(&resp);
                        break;
                        }
                    default:{
                        break;}
                    }
                }
                while (swuart_isTransmitterBusy())
                    ;
            }
            // Packet timeout
            else{
                swuart_clearRxFifo();
            }
        } 
        // This packet is not for me
        else {
            swuart_halt();
            swuart_clearRxFifo();
            _delay_ms(5);
        }
    } 
}

ISR(TIM1_OVF_vect){
    systick_250ms_counter++;
    if(systick_250ms_counter >= BALLANCE_CHECK_PERIOD){
        systick_250ms_counter = 0;
        do_ballance();
    }
}

void do_ballance(){
    if(ballanceV < adc_getVcc()){
        // Dischange FET ON
        PORTB |= (1 << DISCHARGE_PORT);
        status = STATUS_BALLANC_INPROGGRESS;
    } else {
        PORTB &= (1 << DISCHARGE_PORT);
        // Ballancing Done, Stop TIMER1
        TCNT1 &= ~3;
        status = STATUS_BALLANC_DONE;
    }
}
