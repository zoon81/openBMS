#include "systick.h"

const nrf_drv_timer_t TIMER_LED = NRF_DRV_TIMER_INSTANCE(1);
volatile uint32_t systick_ms;

void timer_event_handler(nrf_timer_event_t event_type, void* p_context)
{
    switch(event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
            systick_ms++;
            break;
        
        default:
            //Do nothing.
            break;
    }    
}

void systick_init(){
    
    uint32_t err_code = nrf_drv_timer_init(&TIMER_LED, NULL, timer_event_handler);
    APP_ERROR_CHECK(err_code);
    
   uint32_t time_ticks = nrf_drv_timer_ms_to_ticks(&TIMER_LED, 1);
    
    nrf_drv_timer_extended_compare(
         &TIMER_LED, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
    
    nrf_drv_timer_enable(&TIMER_LED);
}

uint32_t systick_getSystick(){
    return systick_ms;
}

uint32_t systick_getElapsedtime(uint32_t start_timebase){
    uint32_t elapsed_time;
    uint32_t systick_cache = systick_ms;
    if(start_timebase > systick_cache){
        elapsed_time = SYSTICK_MAXVALUE - start_timebase + systick_cache; 
    } else {
        elapsed_time = systick_cache - start_timebase;
    }

    return elapsed_time;
}