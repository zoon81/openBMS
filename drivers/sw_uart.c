#include "sw_uart.h"
#include "fifo.h"

#define MODE_TRANSMITTER 0
#define MODE_RECEIVER 1
#define TIMER_PRESCALER_8 (1 << CS01)
#define TMPVAL 0x55
#define TX_BUFFER_LAST_INDEX (TX_BUFFER_SIZE - 1)

//FiFo
struct fifo_t tx_fifo;
uint8_t tx_buffer[TX_BUFFER_SIZE];

struct fifo_t rx_fifo;
uint8_t rx_buffer[TX_BUFFER_SIZE];

volatile uint8_t status;
volatile uint8_t transreceiver_mode = MODE_RECEIVER;
volatile uint8_t bytemask;
volatile uint8_t tmp;

void swuart_init() {
    SWUART_TX_PORT |= (1 << SWUART_TX_PIN);
    SWUART_TX_DDR |= (1 << SWUART_TX_PIN);

    //FiFo
    tx_fifo.buffer = &tx_buffer[0];
    tx_fifo.used = 0;
    tx_fifo.head = 0;
    tx_fifo.tail = 0;
    tx_fifo.size = TX_BUFFER_SIZE;

    rx_fifo.buffer = &rx_buffer[0];
    rx_fifo.used = 0;
    rx_fifo.head = 0;
    rx_fifo.tail = 0;
    rx_fifo.size = RX_BUFFER_SIZE;

    
    OCR0A = WAIT_BETWEEN_SAMPLES;
    TIMSK = (1 << OCIE0A);
    TCCR0A = (1 << WGM01);
    sei();
    TCNT0 = 0;
    bytemask = 0xFF;
    status = SW_UART_STATUS_READY;
}

void swuart_transmit(uint8_t data){
    // wait until we get free space in fifo
    while( fifo_push(&tx_fifo, data) ){};
    //transreceiver_mode = MODE_TRANSMITTER;
    status = SW_UART_STATUS_TRANSMIT | SW_UART_STATUS_BUSY;
    TCCR0B |= TIMER_PRESCALER_8;
}

uint8_t swuart_isTransmitterBusy(){
    return status & SW_UART_STATUS_BUSY;
}

void swuart_receive(){
    //External Interrupt Request 0 Enable
    GIMSK |= (1 << INT0);
    //transreceiver_mode = MODE_RECEIVER;
    status = SW_UART_STATUS_RECEIVE;
}

uint8_t swuart_availableByte(){
    return rx_fifo.used;
}

uint8_t swuart_getReceivedByte(){
    uint8_t data;
    fifo_pull(&rx_fifo, &data);
    return data;
}

ISR(TIM0_COMPA_vect){
    if (status & SW_UART_STATUS_TRANSMIT){
        //TRASNSMITTER
        if (tx_fifo.used <= 0){
            TCCR0B = TCCR0B & 0xF8; //no more data, shut timer down
            status = SW_UART_STATUS_READY;
        }
        else{
            //generate START condition
            if (bytemask == 0xFF){
                SWUART_TX_PORT &= ~(1 << SWUART_TX_PIN);
                bytemask = 0; //Start with the LSB
            } else {
                //generating STOP
                if (bytemask == 0x08) {
                    SWUART_TX_PORT |= (1 << SWUART_TX_PIN);
                    bytemask = 0xFF;
                    //handling fifo
                    fifo_steptail(&tx_fifo);
                    bytemask = 0xFF;
                }
                //Sending data bit
                else {
                    
                    if (tx_fifo.buffer[tx_fifo.tail] & (1 << bytemask)) {
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
            uint8_t pin_value = (SWUART_RX_P_IN & (1 << SWUART_RX_PIN)) ? 1 : 0;
            tmp |=  pin_value << bytemask;
            bytemask++;
        } else {
            fifo_push(&rx_fifo, tmp);
            TCCR0B = TCCR0B & 0xF8;     //Shut timer down
            GIMSK |= (1 << INT0);       //External Interrupt Request 0 Enable
            bytemask = 0xFF;            //set for transmitter
        }
    }
}
//Start cond. detection
ISR(INT0_vect){
    //Fireing up timer
    OCR0A = WAIT_AFTER_START_COND;
    TCNT0 = 0;
    TCCR0B |= TIMER_PRESCALER_8;
    //Disabling future INT0 interupts until receiving packet
    GIMSK &= ~(1 << INT0);
    bytemask = 0;
    tmp = 0;
}
