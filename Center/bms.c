#include "bms.h"
#include "mem_manager.h"
#include "ina219.h"
#include "frame.h"
#include "ble_bms_service.h"

Bms_data_t bmsData;

static void bms_setCellVoltage(uint8_t index, CellVoltage_t cv);

void bms_init(uint8_t cellCount){
    nrf_mem_init();
    ina219_init();
    // Init BMS Data to default values
    bmsData.packVoltage.Voltage_mV = 0;
    bmsData.packVoltage.timestamp = 0;
    bmsData.packVoltage.status = BMS_MEANSURE_STATUS_INVALID;

    bmsData.packCurrent.Current_mA = 0;
    bmsData.packCurrent.timestamp = 0;
    bmsData.packCurrent.status = BMS_MEANSURE_STATUS_INVALID;

    bmsData.cellData.cellCount = cellCount;
    bmsData.cellData.index = 0;
    // Allocate memory for CellVoltage
    bmsData.cellData.cellVoltage =  nrf_calloc(bmsData.cellData.cellCount, sizeof(bmsData.cellData.cellVoltage));
    // record Database;     
}

void bms_collectData(){
    uint32_t basetime = systick_getSystick();
    uint8_t ina219_ack = 1;
    while( !(ina219_startConversion()) ){
        if(systick_getElapsedtime(basetime) > TIMEOUT_MS_COM_INA){
            ina219_ack = 0;
            break;
        }
    }
    // Get cellvoltages until ina219 do his job
    for(uint8_t i =0; i < bmsData.cellData.cellCount; i++){
        basetime = systick_getSystick();
        cell_requestCellVoltage(CELL1_ADDR + i);
        CellVoltage_t cv;
        cv.status = BMS_MEANSURE_STATUS_VALID;
        while( !(cell_getVoltage(&(cv.cellVoltage_mV))) ){
            if(systick_getElapsedtime(basetime) > TIMEOUT_MS_COM_CELL){
                cv.status = BMS_MEANSURE_STATUS_TIMEOUT;
                break;
            }
        }
        bms_setCellVoltage(i,cv);
    }
    // We read data from ina219 if we get ack for startConversion()
    if(ina219_ack){
        // BusVoltage
        while(ina219_requestBusVoltage() != NRF_SUCCESS){
            if(systick_getElapsedtime(basetime) > TIMEOUT_MS_COM_INA){
                PackVoltage_t packVoltage = {0 , systick_getSystick(), BMS_MEANSURE_STATUS_TIMEOUT};
                bms_setPackVoltage(packVoltage);
                break;
            }
        }
        // Current
        basetime = systick_getSystick();
        while( ina219_requestCurrent() != NRF_SUCCESS){
            if(systick_getElapsedtime(basetime) > TIMEOUT_MS_COM_INA){
                PackCurrent_t packCurrent = {0 , systick_getSystick(), BMS_MEANSURE_STATUS_TIMEOUT};
                bms_setPackCurrent(packCurrent);
                break;
            }
        }
        // PowerDown
        basetime = systick_getSystick();
        while( ina219_powerDown() != NRF_SUCCESS){
            if(systick_getElapsedtime(basetime) > TIMEOUT_MS_COM_INA){
                break;
            }
        }
    }
}
// this function is gluing bms and ble modules togeather
void bms_updateBLEData(){
    if(bmsData.packVoltage.status == BMS_MEANSURE_STATUS_VALID){
      uint32_t retcode =   ble_bms_updatePackVoltage(bmsData.packVoltage.Voltage_mV);
      if(retcode != NRF_SUCCESS){
            APP_ERROR_CHECK(retcode);
        }
    }
    if(bmsData.packCurrent.status == BMS_MEANSURE_STATUS_VALID){
        uint32_t retcode = ble_bms_updatePackCurrent(bmsData.packCurrent.Current_mA);
        if(retcode != NRF_SUCCESS){
            APP_ERROR_CHECK(retcode);
        }

    }
    // To-Do: CellVoltage need to be update too
    uint16_t buffer[bmsData.cellData.cellCount];
    for(uint8_t i =0; i < bmsData.cellData.cellCount; i++){
        if(bmsData.cellData.cellVoltage[i].status == BMS_MEANSURE_STATUS_VALID){
            buffer[i] = bmsData.cellData.cellVoltage[i].cellVoltage_mV;
        } else {
            buffer[i] = 0;
        }
    }
    uint32_t retcode = ble_bms_updateCellVoltage(buffer, bmsData.cellData.cellCount);
    if(retcode != NRF_SUCCESS){
        APP_ERROR_CHECK(retcode);
    }
}

void bms_setPackVoltage(PackVoltage_t packVoltage){
    bmsData.packVoltage = packVoltage;
}

uint16_t bms_getPackVoltageValue(){
    return bmsData.packVoltage.Voltage_mV;
}

void bms_setPackCurrent(PackCurrent_t packCurrent){
    bmsData.packCurrent = packCurrent;
}

int16_t bms_getPackCurrentValue(){
    return bmsData.packCurrent.Current_mA;
}

static void bms_setCellVoltage(uint8_t index, CellVoltage_t cv){
    bmsData.cellData.cellVoltage[index] = cv;
}

CellVoltage_t bms_getCellVoltage(uint8_t index){
    return bmsData.cellData.cellVoltage[index];
}
