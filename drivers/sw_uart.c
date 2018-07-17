#include "sw_uart.h"

#define MODE_TRANSMITTER 0
#define MODE_RECEIVER 1
#define TIMER_PRESCALER_8 (1 << CS01)
#define TMPVAL 0x55


volatile uint8_t tx_buffer[TX_BUFFER_SIZE];
volatile uint8_t tx_buffer_index_head = 0;
volatile uint8_t tx_buffer_index_tail = 0;

volatile uint8_t transreceiver_mode = MODE_RECEIVER;

volatile uint8_t bytemask;

void swuart_init() {
    SWUART_TX_PORT |= (1 << SWUART_TX_PIN);
    SWUART_TX_DDR |= (1 << SWUART_TX_PIN);

    rx_buffer_index_head = 0;
    rx_buffer_index_tail = 0;
    
    OCR0A = WAIT_BETWEEN_SAMPLES;
    TIMSK = (1 << OCIE0A);
    TCCR0A = (1 << WGM01);
    sei();
    TCNT0 = 0;
}

void swuart_transmit(uint8_t data){
    tx_buffer[tx_buffer_index_head] = data;
    tx_buffer_index_head++;
    if(tx_buffer_index_head > (TX_BUFFER_SIZE - 1)){
        tx_buffer_index_head = 0;
    }
    transreceiver_mode = MODE_TRANSMITTER;
    bytemask = 0xFF;                                    //If this is 0xFF, then we need to generate the start condition
    TCCR0B |= TIMER_PRESCALER_8;
}

void swuart_receive(){
    //External Interrupt Request 0 Enable
    GIMSK |= (1 << INT0);
    transreceiver_mode = MODE_RECEIVER;
}

ISR(TIM0_COMPA_vect){
    if (transreceiver_mode == MODE_TRANSMITTER){
        //TRASNSMITTER
        if (tx_buffer_index_head == tx_buffer_index_tail){
            TCCR0B = TCCR0B & 0xF8; //no more data, shut timer down
        }
        else{
            //generate START condition
            if (bytemask == 0xFF){
                PORTB |= (1 << PB0);
                PORTB &= ~(1 << PB0);
                SWUART_TX_PORT &= ~(1 << SWUART_TX_PIN);
                bytemask = 0; //Start with the LSB
            } else {
                //generating STOP
                if (bytemask == 0x08) {
                    SWUART_TX_PORT |= (1 << SWUART_TX_PIN);
                    bytemask = 0xFF;
                    tx_buffer_index_tail++;
                }
                //Sending data bit
                else {
                    PORTB |= (1 << PB3);
                    PORTB &= ~(1 << PB3);
                    if (tx_buffer[tx_buffer_index_tail] & (1 << bytemask)) {
                        SWUART_TX_PORT |= (1 << SWUART_TX_PIN);
                    }
                    else {
                        SWUART_TX_PORT &= ~(1 << SWUART_TX_PIN);
                    }
                    bytemask++;
                }
            }
        }
    }
    //Receiver mode
    else {
        OCR0A = WAIT_BETWEEN_SAMPLES; //UPDATING COUNTER VALUE
        if (bytemask < 8) {
            PORTB |= (1 << PB3);
            PORTB &= ~(1 << PB3);
            uint8_t pin_value = (PINB & (1 << PB2)) ? 1 : 0;
            rx_buffer[rx_buffer_index_head] |=  pin_value << bytemask;
            bytemask++;
        } else {
            rx_buffer_index_head++;
            if (rx_buffer_index_head < RX_BUFFER_SIZE) {
                TCCR0B = TCCR0B & 0xF8;     //Shut timer down
                GIMSK |= (1 << INT0);       //External Interrupt Request 0 Enable
            }
        }
    }
}
//Start cond. detection
ISR(INT0_vect){
    //Fireing up timer
    OCR0A = WAIT_AFTER_START_COND;
    TCNT0 = 0;
    TCCR0B |= TIMER_PRESCALER_8;
    //Disabling future INT0 interupts
    GIMSK &= ~(1 << INT0);
    bytemask = 0;
    rx_buffer[rx_buffer_index_head] = 0;
}
