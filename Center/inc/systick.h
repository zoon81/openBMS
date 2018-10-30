#ifndef INC_SYSTICK_H
#define INC_SYSTICK_H

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_drv_timer.h"
#include "app_error.h"

#define APP_TIMER_PRESCALER             0
#define SYSTICK_MAXVALUE                4294967295 // 32bit unsiged integer maximim value

void systick_init();
uint32_t systick_getSystick();
uint32_t systick_getElapsedtime(uint32_t start_timebase);

#endif