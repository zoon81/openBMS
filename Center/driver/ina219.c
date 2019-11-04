#include "ina219.h"
#include "bms.h"
#include "math.h"
#include "systick.h"

uint16_t cfg_reg_value;
uint8_t twi_data_buffer[3];
double current_lsb;

static void ina219_parseBusVoltageResponse();
static void ina219_parseCurrentResponse();
static void ina219_readWord();
static uint8_t ina219_writeWord(uint8_t regaddr, uint16_t value);

void ina219_init(){
    twi_init();
    ina219_writeWord(REG_ADDR_CONFIG, INA_CONFIG_REG_INIT_VALUE);
    double max_exp_current = (double)CFG_PGA_CAL / (double)Rshunt;
    current_lsb = max_exp_current / 32768;
    double calibration_reg_ = 0.04096 / (current_lsb * (double)Rshunt);
    uint16_t calibration_reg = trunc(calibration_reg_);
    while( !(ina219_writeWord(REG_ADDR_CALIBRATION, calibration_reg)));
}

// Start a single triggered conversion
uint8_t ina219_startConversion(){
    uint16_t conf_busVoltageTriggered = INA_CONFIG_REG_INIT_VALUE | CFG_OPMODE_BUS_AND_SHUTVOLTAGE_TRIGERED;
    return ina219_writeWord(REG_ADDR_CONFIG, conf_busVoltageTriggered);
}

uint8_t ina219_powerDown(){
    return ina219_writeWord(REG_ADDR_CONFIG, INA_CONFIG_REG_INIT_VALUE);
}

// Bus Voltage functions
ret_code_t ina219_requestBusVoltage(){
    uint8_t reg_addr_bus_voltage = REG_ADDR_BUSVOLTAGE;
    ret_code_t retcode = twi_tx(INA219_SLAVE_ADDRESS, &reg_addr_bus_voltage, 1, true);
    if(retcode == NRF_SUCCESS){
        // To-Do: Unregister eventhandler functions
        twi_setTxDoneHandler(ina219_readWord);
        twi_setRxDoneHandler(ina219_parseBusVoltageResponse);
    }
    return retcode;
}

static void ina219_parseBusVoltageResponse(){
    // To-Do: This is not a real timestamp of a conversion
    uint32_t timestamp = systick_getSystick();
    uint16_t bus_voltage_reg = twi_data_buffer[0] << 8 | twi_data_buffer[1];
    uint16_t bus_voltage;
    if((bus_voltage_reg & MASK_BVR_CONVERSION_READY) && !(bus_voltage_reg & MASK_BVR_OVERFLOW) ){
        bus_voltage_reg = bus_voltage_reg >> 3;
        // To-Do: bus_voltage_reg LSB = 4mV, This is true only if we select 32V BusV
        bus_voltage = bus_voltage_reg * 4;
    }
    PackVoltage_t pv = {bus_voltage, timestamp, BMS_MEANSURE_STATUS_VALID};
    bms_setPackVoltage(pv);
}

// Current functions
ret_code_t ina219_requestCurrent(){
    uint8_t reg_addr_current = REG_ADDR_CURRENT;
    ret_code_t retcode = twi_tx(INA219_SLAVE_ADDRESS, &reg_addr_current, 1, true);
    if(retcode == NRF_SUCCESS){
        twi_setTxDoneHandler(ina219_readWord);
        twi_setRxDoneHandler(ina219_parseCurrentResponse);
    }
    return retcode;
}

static void ina219_parseCurrentResponse(){
    // To-Do: This is not a real timestamp of a conversion
    uint32_t timestamp = systick_getSystick();
    uint16_t current_reg = twi_data_buffer[0] << 8 | twi_data_buffer[1];
    int16_t current;
    if(current_reg & MASK_CR_SIGN){
        // Positive current value
        current = trunc( (current_reg & 0x7FFF) * (current_lsb * 1000));
    } else {
        // Negative current value stored in second complement
        current = ~((current_reg & 0x7FFF) + 1);
        current = current * (current_lsb * 1000);
    }
    PackCurrent_t pc = {current, timestamp, BMS_MEANSURE_STATUS_VALID};
    bms_setPackCurrent(pc);
}
// Called from ISR
static void ina219_readWord(){
    twi_rx(INA219_SLAVE_ADDRESS, &twi_data_buffer[0], 2, false);
}

static uint8_t ina219_writeWord(uint8_t regaddr, uint16_t value){
    if( !(twi_getState())){
        twi_data_buffer[0] = regaddr;
        twi_data_buffer[1] = value >> 8;
        twi_data_buffer[2] = value & 0xFF;
        ret_code_t retcode = twi_tx(INA219_SLAVE_ADDRESS, &twi_data_buffer[0], 3, false);
        if(retcode == NRF_SUCCESS){
            twi_setState(TWI_STATUS_SENDING);
        }
        return 1;
    }
    return 0;
}