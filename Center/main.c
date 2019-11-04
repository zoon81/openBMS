/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the license.txt file.
 */
 
/**
 * This file is the main file for the application described in application note
 * nAN-36 Creating Bluetooth� Low Energy Applications Using nRF51822.
 */

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "ble400.h"
#include "app_scheduler.h"
#include "softdevice_handler.h"
#include "app_timer_appsh.h"
#include "nrf_drv_gpiote.h"

#include "ble_bms_service.h"
#include "ble_gap.h"
#include "ble_init.h"
#include "ble_gatts.h"
#include "frame.h"
#include "systick.h"
#include "bms.h"

#define WAKEUP_BUTTON_PIN               BUTTON_0                                /**< Button used to wake up the application. */

#define ADVERTISING_LED_PIN_NO          LED_0                                   /**< Is on when device is advertising. */
#define CONNECTED_LED_PIN_NO            LED_1                                   /**< Is on when device has connected. */
#define APP_TIMER_MAX_TIMERS            2                                           /**< Maximum number of simultaneously created timers. */
#define APP_TIMER_OP_QUEUE_SIZE         4                                           /**< Size of timer operation queues. */


#define APP_GPIOTE_MAX_USERS            1                                           /**< Maximum number of users of the GPIOTE handler. */
#define BUTTON_DETECTION_DELAY          APP_TIMER_TICKS(50, APP_TIMER_PRESCALER)    /**< Delay from a GPIOTE event until a button is reported as pushed (in number of timer ticks). */
#define DEAD_BEEF                       0xDEADBEEF                                  /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

#define SCHED_MAX_EVENT_DATA_SIZE       sizeof(app_timer_event_t)                   /**< Maximum size of scheduler events. Note that scheduler BLE stack events do not contain any data, as the events are being pulled from the stack in the event handler. */
#define SCHED_QUEUE_SIZE                10                                          /**< Maximum number of events in the scheduler queue. */

// Application timer ID.
APP_TIMER_DEF(m_led_a_timer_id);
APP_TIMER_DEF(m_systick_timer_id);
uint16_t battV;

// Persistent storage system event handler
void pstorage_sys_event_handler (uint32_t p_evt);

/**@brief Function for error handling, which is called when an error has occurred.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of error.
 *
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the handler is called.
 * @param[in] p_file_name Pointer to the file name.
 */
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    // This call can be used for debug purposes during application development.
    // @note CAUTION: Activating this code will write the stack to flash on an error.
    //                This function should NOT be used in a final product.
    //                It is intended STRICTLY for development/debugging purposes.
    //                The flash write will happen EVEN if the radio is active, thus interrupting
    //                any communication.
    //                Use with care. Un-comment the line below to use.
    ble_debug_assert_handler(error_code, line_num, p_file_name);

    // On assert, the system can only recover with a reset.
    //NVIC_SystemReset();
}


/**@brief Callback function for asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in]   line_num   Line number of the failing ASSERT call.
 * @param[in]   file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}


/**@brief Function for the LEDs initialization.
 *
 * @details Initializes all LEDs used by the application.
 */
// static void leds_init(void)
// {
//     nrf_gpio_cfg_output(ADVERTISING_LED_PIN_NO);
//     nrf_gpio_cfg_output(CONNECTED_LED_PIN_NO);
//     nrf_gpio_cfg_output(LEDBUTTON_LED_PIN_NO);
// }

/**@brief Function for handling the Connection Parameters Module.
 *
 * @details This function will be called for all events in the Connection Parameters Module which
 *          are passed to the application.
 *          @note All this function does is to disconnect. This could have been done by simply
 *                setting the disconnect_on_fail config parameter, but instead we use the event
 *                handler mechanism to demonstrate its use.
 *
 * @param[in]   p_evt   Event received from the Connection Parameters Module.
 */


// Writing properties of battery pack
static void props_write_handler(ble_bms_t * p_bms, uint16_t value){
    // if (led_state)
    // {
    //     nrf_gpio_pin_set(LEDBUTTON_LED_PIN_NO);
    // }
    // else
    // {
    //     nrf_gpio_pin_clear(LEDBUTTON_LED_PIN_NO);
    // }
    value -= 1;
    ble_gatts_value_t gattVal;
    gattVal.len = 2;
    gattVal.offset = 0;
    gattVal.p_value = (uint8_t*) &value;

    uint32_t err_code = sd_ble_gatts_value_set(p_bms->conn_handle,
                                            p_bms->bat_pack_desc_char_handles.value_handle,
                                            &gattVal);
    if (err_code != NRF_SUCCESS){
		APP_ERROR_CHECK(err_code);
    }
}

/**@brief Function for initializing services that will be used by the application.
 */
static void services_init(void)
{
    uint32_t err_code;
    ble_bms_init_t init;
    
    init.props_write_handler = props_write_handler;
    
    err_code = ble_bms_init(&m_bms, &init);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for the Event Scheduler initialization.
 */
static void scheduler_init(void)
{
    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}


// static void button_event_handler(uint8_t pin_no, uint8_t button_action)
// {
//     uint32_t err_code;
    
//     switch (pin_no)
//     {
//         case LEDBUTTON_BUTTON_PIN_NO:
//             err_code = ble_lbs_on_button_change(&m_bms, button_action);
//             if (err_code != NRF_SUCCESS &&
//                 err_code != BLE_ERROR_INVALID_CONN_HANDLE &&
//                 err_code != NRF_ERROR_INVALID_STATE)
//             {
//                 APP_ERROR_CHECK(err_code);
//             }
//             break;

//         default:
//             APP_ERROR_HANDLER(pin_no);
//             break;
//     }
// }

/**@brief Function for initializing the GPIOTE handler module.
 */
static void gpiote_init(void)
{
//    APP_GPIOTE_INIT(APP_GPIOTE_MAX_USERS);
}


/**@brief Function for initializing the button handler module.
 */
// static void buttons_init(void)
// {
//     // Note: Array must be static because a pointer to it will be saved in the Button handler
//     //       module.
//     static app_button_cfg_t buttons[] =
//     {
//         {WAKEUP_BUTTON_PIN, false, BUTTON_PULL, NULL},
//         {LEDBUTTON_BUTTON_PIN_NO, false, BUTTON_PULL, button_event_handler}
//     };

//     app_button_init(buttons, sizeof(buttons) / sizeof(buttons[0]), BUTTON_DETECTION_DELAY);
// }


/**@brief Function for the Power manager.
 */
static void power_manage(void)
{
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module.
 */
static void timers_init(void){
    // Initialize timer module, making it use the scheduler
    APP_TIMER_APPSH_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS, true);
}

void bms_collectData_scheduler_event_handler(void *p_event_data, uint16_t event_size){
    bms_collectData();
}

void bms_updateBLEData_scheduler_event_handler(void *p_event_data, uint16_t event_size){
    bms_updateBLEData();
}

// Timeout handler for the repeated timer
static void timer_handler(void * p_context){
    app_sched_event_put(NULL, 0, bms_collectData_scheduler_event_handler);
    if(m_bms.conn_handle != BLE_CONN_HANDLE_INVALID){
        app_sched_event_put(NULL, 0, bms_updateBLEData_scheduler_event_handler);
        // ble_bms_on_packVoltage_change(&m_bms, battV);
        // battV++;
    }
}

// Create timers
static void create_timers()
{
    uint32_t err_code;

    // Create timers
    err_code = app_timer_create(&m_led_a_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                timer_handler);                             
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for starting timers.
*/
static void timers_start(void)
{
    app_timer_start(m_led_a_timer_id, APP_TIMER_TICKS(1000, APP_TIMER_PRESCALER), NULL);
}

void wakeup_button_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    //nrf_drv_gpiote_out_toggle(PIN_OUT);
}
/**
 * @brief Function for configuring: PIN_IN pin for input, PIN_OUT pin for output, 
 * and configures GPIOTE to give an interrupt on pin change.
 */
static void gpio_init(void){
    ret_code_t err_code;

    err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);
    
    nrf_drv_gpiote_out_config_t out_config = GPIOTE_CONFIG_OUT_SIMPLE(false);

    err_code = nrf_drv_gpiote_out_init(ADVERTISING_LED_PIN_NO, &out_config);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    in_config.pull = NRF_GPIO_PIN_PULLUP;

    err_code = nrf_drv_gpiote_in_init(WAKEUP_BUTTON_PIN, &in_config, wakeup_button_pin_handler);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_event_enable(WAKEUP_BUTTON_PIN, true);
}

/**@brief Function for application main entry.
 */

// LASTLOG
// Bms Data show invalid value in cellVoltage
int main(void)
{
    // Initialize
    frame_init();
    //leds_init();
    timers_init();
    gpio_init();
    //buttons_init();
    ble_stack_init();
    scheduler_init();    
    gap_params_init();
    services_init();
    advertising_init();
    conn_params_init();
    sec_params_init();
    battV = 0;
    // Start execution
    // Create application timer instances.
    create_timers();
    systick_init();
    timers_start();
    advertising_start();

    bms_init(CELL_COUNT);
    bms_collectData();

    

    // Enter main loop
    for (;;)
    {
        app_sched_execute();
        power_manage();
    }
}

/**
 * @}
 */