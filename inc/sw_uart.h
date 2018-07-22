#ifndef INC_SWUART_H
#define INC_SWUART_H
#define F_CPU 4800000UL

#include <avr/io.h>
#include <avr/interrupt.h>

//THIS IS ONLY FOR 9600 BAUDRATE
#define BAUD_RATE 9600UL
#define TIMER_CLK_PRESCALER 8
// To-Do : Fine tune required
#define TCNT_CAL 5
#define WAIT_BETWEEN_SAMPLES ((F_CPU / (TIMER_CLK_PRESCALER * BAUD_RATE)) - TCNT_CAL)
//#define WAIT_BETWEEN_SAMPLES 57
#define WAIT_AFTER_START_COND (WAIT_BETWEEN_SAMPLES + (WAIT_BETWEEN_SAMPLES / 2))

#define SWUART_TX_DDR DDRB
#define SWUART_TX_PORT PORTB
#define SWUART_TX_PIN PB4
#define SWUART_RX_DDR DDRB
#define SWUART_RX_PORT PORTB
#define SWUART_RX_PIN PB1

#define TX_BUFFER_SIZE 4
#define RX_BUFFER_SIZE 4
volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
volatile uint8_t rx_buffer_index_head;
volatile uint8_t rx_buffer_index_tail;

void swuart_init();
void swuart_transmit(uint8_t data);
void swuart_receive();


#endif