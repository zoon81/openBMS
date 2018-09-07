#ifndef INC_MAIN_H
#define INC_MAIN_H

#include <avr/io.h>
<<<<<<< HEAD
#include <avr/sleep.h>
#include <avr/interrupt.h>
=======
>>>>>>> 0af11edb2c3301e8d9ec7259e5c7c1e7211edd48

#define DEVICE_ADDRESS 0x35
#define BROADCAST_ADDRESS 0xF5

<<<<<<< HEAD
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

=======
>>>>>>> 0af11edb2c3301e8d9ec7259e5c7c1e7211edd48
#endif