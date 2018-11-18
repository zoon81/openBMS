#include "uart.h"
#include "frame.h"

void (*uart_InCommingData_handler)(uint8_t);

// General UART IRQ handler
void uart_irq_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
    else if(p_event->evt_type == APP_UART_DATA_READY){
        uint8_t data;
        app_uart_get(&data);
        uart_InCommingData_handler(data);
    }
}

uint32_t uart_init(){

    uart_InCommingData_handler = NULL;
    uint32_t err_code;
    const app_uart_comm_params_t comm_params =
      {
          RX_PIN_NUMBER,
          TX_PIN_NUMBER,
          RTS_PIN_NUMBER,
          CTS_PIN_NUMBER,
          APP_UART_FLOW_CONTROL_DISABLED,
          false,
          UART_BAUDRATE_BAUDRATE_Baud9600
      };

    APP_UART_FIFO_INIT(&comm_params,
                         UART_RX_BUF_SIZE,
                         UART_TX_BUF_SIZE,
                         uart_irq_handle,
                         APP_IRQ_PRIORITY_LOW,
                         err_code);

    return err_code;
}

// Set  UART RX and TX pin
void uart_setRxTxPin(uint8_t rx_pin, uint8_t tx_pin){

    nrf_gpio_cfg_output(tx_pin);
    nrf_gpio_pin_set(tx_pin);
    nrf_gpio_cfg_input(rx_pin, NRF_GPIO_PIN_NOPULL);
    nrf_uart_txrx_pins_set(NRF_UART0, tx_pin, rx_pin);

}

// Set the handler function for incomming data over UART
void uart_setIncommingDataHandler( void (*inCommingDataHandler)(uint8_t) ){
    uart_InCommingData_handler = inCommingDataHandler;
}