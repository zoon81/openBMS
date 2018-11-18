#include "twi.h"

/* TWI instance. */
static const nrf_drv_twi_t m_twi0_inst = NRF_DRV_TWI_INSTANCE(0);

void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{   
    ret_code_t err_code;
    switch(p_event->type)
    {
        case NRF_DRV_TWI_RX_DONE:
            
            break;
        case NRF_DRV_TWI_TX_DONE:
           
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
       .scl                = TWI0_CONFIG_SCL,
       .sda                = TWI0_CONFIG_SDA,
       .frequency          = TWI0_CONFIG_FREQUENCY,
       .interrupt_priority = TWI0_CONFIG_IRQ_PRIORITY
    };
    
    err_code = nrf_drv_twi_init(&m_twi0_inst, &twi0_config, twi_handler, NULL);
    APP_ERROR_CHECK(err_code);
    
    nrf_drv_twi_enable(&m_twi0_inst);
}

void twi_tx(uint8_t address, uint8_t const *p_data,uint32_t length,bool xfer_pending){
    nrf_drv_twi_tx(&m_twi0_inst, address, p_data, length, xfer_pending);
}