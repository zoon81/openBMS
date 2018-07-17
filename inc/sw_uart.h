#ifndef INC_SWUART_H
#define INC_SWUART_H

#include <avr/io.h>
#include <avr/interrupt.h>

//THIS IS ONLY FOR 9600 BAUDRATE
#define BAUD_RATE 9600
// To-Do : Fine tune required
#define WAIT_BETWEEN_SAMPLES 104
#define WAIT_AFTER_START_COND 156

#define SWUART_TX_DDR DDRB
#define SWUART_TX_PORT PORTB
#define SWUART_TX_PIN PB4
#define SWUART_RX_DDR DDRB
#define SWUART_RX_PORT PORTB
#define SWUART_RX_PIN PB2

#define TX_BUFFER_SIZE 4
#define RX_BUFFER_SIZE 4
volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
volatile uint8_t rx_buffer_index_head;
volatile uint8_t rx_buffer_index_tail;

void swuart_init();
void swuart_transmit(uint8_t data);
void swuart_receive();


#endif