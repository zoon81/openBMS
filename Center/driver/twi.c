#include "twi.h"

/* TWI instance. */
static const nrf_drv_twi_t m_twi0_inst = NRF_DRV_TWI_INSTANCE(0);
void (*twi_txDone_eventHandler)();
void (*twi_rxDone_eventHandler)();
uint8_t twi_status = TWI_STATUS_BUSY;

void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{   
    ret_code_t err_code;
    switch(p_event->type)
    {
        case NRF_DRV_TWI_RX_DONE:
            twi_status = TWI_STATUS_IDLE;
            if(twi_rxDone_eventHandler != NULL){
                twi_rxDone_eventHandler();
            }
            break;
        case NRF_DRV_TWI_TX_DONE:
            twi_status = TWI_STATUS_IDLE;
            if(twi_txDone_eventHandler != NULL){
                twi_txDone_eventHandler();
            }
            /* Read 4 bytes from the specified address. */
            //err_code = nrf_drv_twi_rx(&m_twi_mma_7660, MMA7660_ADDR, (uint8_t*)&m_sample, sizeof(m_sample), false);
            //APP_ERROR_CHECK(err_code);
            break;
        default:
            break;        
    }   
}

/**
 * @brief UART initialization.
 */
void twi_init (void)
{
    ret_code_t err_code;
    
    const nrf_drv_twi_config_t twi0_config = {
       .scl                = TWI_SCL_PIN,
       .sda                = TWI_SDA_PIN,
       .frequency          = TWI0_CONFIG_FREQUENCY,
       .interrupt_priority = TWI0_CONFIG_IRQ_PRIORITY
    };
    
    err_code = nrf_drv_twi_init(&m_twi0_inst, &twi0_config, twi_handler, NULL);
    twi_txDone_eventHandler = NULL;
    twi_rxDone_eventHandler = NULL;
    APP_ERROR_CHECK(err_code);
    
    nrf_drv_twi_enable(&m_twi0_inst);
    twi_status = TWI_STATUS_IDLE;
}

void twi_setTxDoneHandler(void (*txDoneHandler)()){
    twi_txDone_eventHandler = txDoneHandler;
}

void twi_setRxDoneHandler( void (*rxDonenHandler) ()){
    twi_rxDone_eventHandler = rxDonenHandler;
}

ret_code_t twi_tx(uint8_t address, uint8_t const *p_data,uint32_t length,bool xfer_pending){
    // Wait for pending operations
    return nrf_drv_twi_tx(&m_twi0_inst, address, p_data, length, xfer_pending);
}

ret_code_t twi_rx(uint8_t address, uint8_t *incommingdata, uint8_t length, bool xref_pending){
    return nrf_drv_twi_rx(&m_twi0_inst, address, incommingdata, length, xref_pending);
}

uint8_t twi_getState(){
    return twi_status;
}

void twi_setState(uint8_t state){
    twi_status = state;
}