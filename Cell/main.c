#include "main.h"
#include "util/delay.h"
#include "sw_uart.h"
#include "eeprom_api.h"
#include "packet.h"
#include "adc.h"
#include "util.h"

// #define ENABLE_DEBUG_LED
#define DEBUG_LED_PIN PB3

void do_ballance();

uint8_t status = STATUS_IDLE;
uint16_t ballanceV = 0;
uint8_t systick_250ms_counter = 0;

void main(){
    //Balance port config
    DDRB |= (1 << DISCHARGE_PORT);
    
    // Check for badgap reference calibration value
    uint8_t bgrefcalkey = EEPROM_read(EE_BGREFCALKEY_ADDR);
    if(bgrefcalkey == EE_BGREFCALKEY){
        // load calibration value from eeprom
        int16_t calibration_value = EEPROM_read(EE_BGREFCALVALUE_L_ADDR);
        calibration_value |= EEPROM_read(EE_BGREFCALVALUE_H_ADDR) << 8;
        adc_setCalibrationValue(calibration_value);
    } else{
        // Calibrating internal voltage reference, we expect that the device is powered from 3300mV
        #ifdef ENABLE_DEBUG_LED
        DDRB |= (1 << DEBUG_LED_PIN);
        PORTB |=  (1 << DEBUG_LED_PIN);
        _delay_ms(1000);
        PORTB &=  ~(1 << DEBUG_LED_PIN);
        DDRB &= ~(1 << DEBUG_LED_PIN);
        #endif
        adc_init();
        _delay_ms(1000);
        adc_getRawVcc();
        uint32_t adcvalue = adc_getAvgRawVcc(32);
        uint32_t tmpvalue = adcvalue * (uint32_t) 3300;
        tmpvalue = tmpvalue / ADC_MAXVALUE;
        int16_t calibration_value = ADC_BANDGAP_mV_stock - tmpvalue;
        
        // Store calibration values
        EEPROM_write(EE_BGREFCALKEY_ADDR, EE_BGREFCALKEY);
        EEPROM_write(EE_BGREFCALVALUE_H_ADDR, calibration_value >> 8);
        EEPROM_write(EE_BGREFCALVALUE_L_ADDR, calibration_value & 0xFF);
        EEPROM_write(0x03, adcvalue >> 8);
        EEPROM_write(0x04, adcvalue & 0xFF);
        //EEPROM_write(0x05, tmpvalue >> 24);
        //EEPROM_write(0x06, tmpvalue >> 16);
        //EEPROM_write(0x07, tmpvalue >> 8);
        //EEPROM_write(0x08, tmpvalue & 0xFF);
        adc_setCalibrationValue(calibration_value);
    }

    // adc_setCalibrationValue(ADC_BANDGAP_mV_cal);
    
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
        // To-Do: Disable sleep mode when cellballancing in progress
        //Turn off adc to save power and go sleep
        adc_deinit();
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_mode();

        // We land here after wake up (INT0 goes low)
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
                    case PACKET_CMD_BAT_V:{
                        adc_init();
                        // delay for internal reference stabilization
                        _delay_ms(2);
                        // The first ADC conversion result after switching voltage reference source maybe inaccurate, and the user is advised to discard this result.
                        adc_getRawVcc();
                        struct packet_t resp;
                        resp.address = PACKET_MASTER_ADDRESS;
                        resp.command = p.command;
                        resp.data = adc_getVcc();
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
                        break;
                    }
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
