#ifndef INC_BMS_H
#define INC_BMS_H

#include "inttypes.h"

#define CELL_COUNT 3
#define TIMEOUT_MS_COM_INA 5
#define TIMEOUT_MS_COM_CELL 25
#define BMS_MEANSURE_STATUS_VALID   0
#define BMS_MEANSURE_STATUS_INVALID 2
#define BMS_MEANSURE_STATUS_TIMEOUT 1

typedef struct{
    uint16_t cellVoltage_mV;
    uint32_t timestamp;
    uint8_t status;
} CellVoltage_t;

typedef struct{
    CellVoltage_t       *cellVoltage;
    uint8_t             cellCount;
    uint8_t             index;
} CellData_t;

typedef struct{
    uint16_t Voltage_mV;
    uint32_t timestamp;
    uint8_t status;
} PackVoltage_t;

typedef struct{
    int16_t Current_mA;
    uint32_t timestamp;
    uint8_t status;
} PackCurrent_t;

typedef struct{
    CellData_t          cellData;
    PackCurrent_t       packCurrent;
    PackVoltage_t       packVoltage;
} Bms_data_t;

struct decoder_s{
    unsigned battery_current    : 11;
    unsigned battery_voltage    : 14;
    unsigned battery_temp       :  7;
};

union record {
    uint32_t record_value;
    struct decoder_s decoder;
};

void bms_init(uint8_t cellCount);
void bms_collectData();
void bms_updateBLEData();
uint16_t bms_getPackVoltageValue();
int16_t bms_getPackCurrentValue();
void bms_setPackCurrent(PackCurrent_t packCurrent);
void bms_setPackVoltage(PackVoltage_t packVoltage);


#endif