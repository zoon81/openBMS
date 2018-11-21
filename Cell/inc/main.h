#ifndef INC_MAIN_H
#define INC_MAIN_H

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#define DEVICE_ADDRESS 0x35
#define BROADCAST_ADDRESS 0xF5

//Ballance settings
#define DISCHARGE_PORT PB3
#define BALLANCE_CHECK_INTERVAL_MS 1000
#define TIMER1_IRQ_PERION 250
#define BALLANCE_CHECK_PERIOD (BALLANCE_CHECK_INTERVAL_MS / TIMER1_IRQ_PERION)

// Device status
#define STATUS_IDLE                 0
#define STATUS_SLEEPP               1
#define STATUS_BALLANC_INPROGGRESS  2
#define STATUS_BALLANC_DONE         4

// EEPROM Data Definitions
#define EE_BGREFCALKEY_ADDR             0x00
#define EE_BGREFCALVALUE_H_ADDR         0x01
#define EE_BGREFCALVALUE_L_ADDR         0x02
#define EE_BGREFCALKEY                  0x59

#endif