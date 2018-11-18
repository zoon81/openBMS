#include "bms.h"
#include "mem_manager.h"

Bms_data_t bmsData;

void bms_init(uint8_t cellCount){
    nrf_mem_init();
    bmsData.cellData.cellCount = CELL_COUNT;
    bmsData.cellData.cellVoltage =  nrf_calloc(bmsData.cellData.cellCount, sizeof(bmsData.cellData.cellVoltage));      
}

void bms_setPackVoltage(uint16_t packVoltage){
    bmsData.packVoltage = packVoltage;
}

uint16_t bms_getPackVoltage(){
    return bmsData.packVoltage;
}

void bms_setPackCurrent(uint16_t packCurrent){
    bmsData.packCurrent = packCurrent;
}

uint16_t bms_getPackCurrent(){
    return bmsData.packCurrent;
}