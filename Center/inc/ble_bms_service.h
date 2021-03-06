#ifndef INC_BLE_BMS_SERVICE_H
#define INC_BLE_BMS_SERVICE_H

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"

#define BMS_UUID_BASE {0x23, 0xD0, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}
#define BMS_UUID_SERVICE 0x3023
#define BMS_UUID_BATPACKDESC_CHAR 0x3025
#define BMS_UUID_BATPACKVOLT_CHAR 0x1526
#define BMS_UUID_BATPACKCURR_CHAR 0x1527
#define BMS_UUID_BATCELLVOLT_CHAR 0x1528
#define BMS_UUID_DATACHANGED_CHAR 0x1529

#define BLE_BMS_DATACHANGED_BATTPACK    0x01
#define BLE_BMS_DATACHANGED_STATUS      0x02

// Forward declaration of the ble_bms_t type. 
typedef struct ble_bms_s ble_bms_t;

// Function type forward declaration
typedef void (*ble_bms_props_write_handler_t) (ble_bms_t * p_bms, uint16_t newprops);

typedef struct
{
    ble_bms_props_write_handler_t props_write_handler;                    /**< Event handler to be called when LED characteristic is written. */
} ble_bms_init_t;

typedef struct ble_bms_s
{
    uint16_t                        service_handle;
    ble_gatts_char_handles_t        bat_pack_desc_char_handles;
    ble_gatts_char_handles_t        bat_pack_props_char_handles;
    ble_gatts_char_handles_t        bat_pack_volt_char_handles;
    ble_gatts_char_handles_t        bat_pack_current_char_handles;
    ble_gatts_char_handles_t        bat_selected_cell_char_handles;
    ble_gatts_char_handles_t        bat_cellVoltage_char_handles;
    ble_gatts_char_handles_t        data_changed_char_handles;
    uint8_t                         uuid_type;
    uint16_t                        conn_handle;
    ble_bms_props_write_handler_t   props_write_handler;
} ble_bms_t;

ble_bms_t               m_bms;
ble_gap_sec_params_t    m_sec_params;                               /**< Security requirements for this application. */
uint16_t                bms_properties;

uint32_t ble_bms_init(ble_bms_t * p_bms, const ble_bms_init_t * p_bms_init);
void ble_bms_on_ble_evt(ble_bms_t * p_bms, ble_evt_t * p_ble_evt);
uint32_t ble_bms_on_packVoltage_change(ble_bms_t * p_bms, uint16_t battery_voltage);

uint32_t ble_bms_updatePackVoltage(uint16_t packV);
uint32_t ble_bms_updatePackCurrent(uint16_t packC);
uint32_t ble_bms_updateCellVoltage(uint16_t *pdata, uint8_t len);


#endif