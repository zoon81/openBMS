#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
//#include "boards.h"
#include "ble400.h"

main(void)
{
  // Configure LED-pins as outputs
  nrf_gpio_range_cfg_output(LED_START, LED_STOP);

  while (true)
  {
    nrf_gpio_pin_clear(LED_0);
    nrf_gpio_pin_set(LED_1);
    nrf_gpio_pin_clear(LED_2);
    nrf_gpio_pin_set(LED_3);
    nrf_gpio_pin_clear(LED_4);
    nrf_delay_ms(300);

    nrf_gpio_pin_set(LED_0);
    nrf_gpio_pin_clear(LED_1);
    nrf_gpio_pin_set(LED_2);
    nrf_gpio_pin_clear(LED_3);
    nrf_gpio_pin_set(LED_4);
    nrf_delay_ms(500);
  }
}