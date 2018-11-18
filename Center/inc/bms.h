#ifndef INC_BMS_H
#define INC_BMS_H

#include "inttypes.h"

#define CELL_COUNT 3

typedef struct{
    uint16_t            *cellVoltage;
    uint8_t             cellCount;
} CellData_t;

typedef struct{
    CellData_t          cellData;
    uint16_t            packCurrent;
    uint16_t            packVoltage;
} Bms_data_t;

#endif