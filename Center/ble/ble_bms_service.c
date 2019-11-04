#include <string.h>
#include "ble_bms_service.h"
#include "nordic_common.h"
#include "ble_srv_common.h"
#include "app_util.h"

// BLE stack event for updating connection handle
static void on_connect(ble_bms_t * p_bms, ble_evt_t * p_ble_evt){
    p_bms->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}

// BLE stack event for updating connection handle
static void on_disconnect(ble_bms_t * p_bms, ble_evt_t * p_ble_evt){
    UNUSED_PARAMETER(p_ble_evt);
    p_bms->conn_handle = BLE_CONN_HANDLE_INVALID;
}

/**@brief Function for handling the Write event.
 *
 * @param[in]   p_lbs       LED Button Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_write(ble_bms_t * p_bms, ble_evt_t * p_ble_evt){
    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
    
    // Properties write, this is a 2byte value
    if ((p_evt_write->handle == p_bms->bat_pack_desc_char_handles.value_handle) &&
        (p_evt_write->len == 2) &&
        (p_bms->props_write_handler != NULL)){
            uint16_t props = p_evt_write->data[1] | (p_evt_write->data[0] << 8);
            p_bms->props_write_handler(p_bms, props);
    }

}

// BLE Event state machine
void ble_bms_on_ble_evt(ble_bms_t * p_bms, ble_evt_t * p_ble_evt)
{
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_bms, p_ble_evt);
            break;
            
        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_bms, p_ble_evt);
            break;
            
        case BLE_GATTS_EVT_WRITE:
            on_write(p_bms, p_ble_evt);
            break;
            
        default:
            // No implementation needed.
            break;
    }
}

static uint32_t batPackDesc_char_add(ble_bms_t * p_bms){
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));
    
    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;
    
    ble_uuid.type = p_bms->uuid_type;
    ble_uuid.uuid = BMS_UUID_BATPACKDESC_CHAR;
    
    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;
    
    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint16_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = sizeof(uint16_t);
    attr_char_value.p_value      = NULL;
    
    return sd_ble_gatts_characteristic_add(p_bms->service_handle, &char_md,
                                               &attr_char_value,
                                               &p_bms->bat_pack_desc_char_handles);
}

static uint32_t batPackVolt_char_add(ble_bms_t * p_bms){
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(cccd_md));
    
    char_md.char_props.read   = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;
    
    ble_uuid.type = p_bms->uuid_type;
    ble_uuid.uuid = BMS_UUID_BATPACKVOLT_CHAR;
    
    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;
    
    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint16_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = sizeof(uint16_t);
    attr_char_value.p_value      = NULL;
    
    return sd_ble_gatts_characteristic_add(p_bms->service_handle, &char_md,
                                               &attr_char_value,
                                               &p_bms->bat_pack_volt_char_handles);
}

static uint32_t batPackCurrent_char_add(ble_bms_t * p_bms){
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
    
    memset(&char_md, 0, sizeof(char_md));
    
    char_md.char_props.read   = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;
    
    ble_uuid.type = p_bms->uuid_type;
    ble_uuid.uuid = BMS_UUID_BATPACKCURR_CHAR;
    
    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;
    
    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(int16_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = sizeof(int16_t);
    attr_char_value.p_value      = NULL;
    
    return sd_ble_gatts_characteristic_add(p_bms->service_handle, &char_md,
                                               &attr_char_value,
                                               &p_bms->bat_pack_current_char_handles);
}

static uint32_t batCellVoltage_char_add(ble_bms_t * p_bms){
    ble_gatts_char_md_t char_md;
    // ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    // memset(&cccd_md, 0, sizeof(cccd_md));

    // BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    //BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    // cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    
    // memset(&char_md, 0, sizeof(char_md));
    
    char_md.char_props.read   = 1;
    // char_md.char_props.notify = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    // char_md.p_cccd_md         = &cccd_md;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;
    
    ble_uuid.type = p_bms->uuid_type;
    ble_uuid.uuid = BMS_UUID_BATCELLVOLT_CHAR;
    
    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;
    
    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint16_t) * 3;
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = sizeof(uint16_t) * 3;
    attr_char_value.p_value      = NULL;
    
    return sd_ble_gatts_characteristic_add(p_bms->service_handle, &char_md,
                                               &attr_char_value,
                                               &p_bms->bat_cellVoltage_char_handles);
}

static uint32_t dataChanged_char_add(ble_bms_t * p_bms){
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    
    memset(&char_md, 0, sizeof(char_md));
    
    char_md.char_props.read   = 1;
    char_md.char_props.notify = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;
    
    ble_uuid.type = p_bms->uuid_type;
    ble_uuid.uuid = BMS_UUID_DATACHANGED_CHAR;
    
    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;
    
    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint8_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = sizeof(uint8_t);
    attr_char_value.p_value      = NULL;
    
    return sd_ble_gatts_characteristic_add(p_bms->service_handle, &char_md,
                                               &attr_char_value,
                                               &p_bms->data_changed_char_handles);
}

uint32_t ble_bms_init(ble_bms_t * p_bms, const ble_bms_init_t * p_bms_init)
{
    uint32_t   err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure
    p_bms->conn_handle          = BLE_CONN_HANDLE_INVALID;
    p_bms->props_write_handler  = p_bms_init->props_write_handler;
    
    // Add service
    ble_uuid128_t base_uuid = {BMS_UUID_BASE};
    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_bms->uuid_type);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
    ble_uuid.type = p_bms->uuid_type;
    ble_uuid.uuid = BMS_UUID_SERVICE;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_bms->service_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
    err_code = batPackDesc_char_add(p_bms);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    err_code = batPackVolt_char_add(p_bms);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    err_code = batPackCurrent_char_add(p_bms);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    err_code = batCellVoltage_char_add(p_bms);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    err_code = dataChanged_char_add(p_bms);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
    return NRF_SUCCESS;
}

// Notification handlers
// This function is notify the client if any changes happend in ble data 
uint32_t ble_bms_on_data_change(ble_bms_t * p_bms, uint8_t changle)
{
    ble_gatts_hvx_params_t params;
    uint16_t len = sizeof(changle);
    
    memset(&params, 0, sizeof(params));
    params.type = BLE_GATT_HVX_NOTIFICATION;
    params.handle = p_bms->data_changed_char_handles.value_handle;
    params.p_data = &changle;
    params.p_len = &len;
    
    return sd_ble_gatts_hvx(p_bms->conn_handle, &params);
}

uint32_t ble_bms_on_packVoltage_change(ble_bms_t * p_bms, uint16_t battery_voltage)
{
    ble_gatts_hvx_params_t params;
    uint16_t len = sizeof(battery_voltage);
    
    memset(&params, 0, sizeof(params));
    params.type = BLE_GATT_HVX_NOTIFICATION;
    params.handle = p_bms->bat_pack_volt_char_handles.value_handle;
    params.p_data = &battery_voltage;
    params.p_len = &len;
    
    return sd_ble_gatts_hvx(p_bms->conn_handle, &params);
}

uint32_t ble_bms_on_packCurrent_change(ble_bms_t * p_bms, int16_t battery_current)
{
    ble_gatts_hvx_params_t params;
    uint16_t len = sizeof(battery_current);
    
    memset(&params, 0, sizeof(params));
    params.type = BLE_GATT_HVX_NOTIFICATION;
    params.handle = p_bms->bat_pack_volt_char_handles.value_handle;
    params.p_data = &battery_current;
    params.p_len = &len;
    
    return sd_ble_gatts_hvx(p_bms->conn_handle, &params);
}

uint32_t ble_bms_updatePackVoltage(uint16_t packV){
    ble_gatts_value_t packV_gatts;
    // Endian Swap
    uint8_t buffer[2] = { packV >> 8, packV & 0xFF };
    packV_gatts.len = 2;
    packV_gatts.offset = 0;
    packV_gatts.p_value = &buffer[0];
    return sd_ble_gatts_value_set(  m_bms.conn_handle,
                                    m_bms.bat_pack_volt_char_handles.value_handle,
                                    &packV_gatts);
}

uint32_t ble_bms_updatePackCurrent(uint16_t packC){
    ble_gatts_value_t packV_gatts;
    // Endian Swap
    uint8_t buffer[2] = { packC >> 8, packC & 0xFF };
    packV_gatts.len = 2;
    packV_gatts.offset = 0;
    packV_gatts.p_value = &buffer[0];
    return sd_ble_gatts_value_set(  m_bms.conn_handle,
                                    m_bms.bat_pack_current_char_handles.value_handle,
                                    &packV_gatts);
}

uint32_t ble_bms_updateCellVoltage(uint16_t *pdata, uint8_t len){
    ble_gatts_value_t cellV_gatts;
    //uint16_t tmp[] = {0x0001, 0x0002, 0x0003, 0x0004};
    cellV_gatts.len = 6;
    cellV_gatts.offset = 0;
    cellV_gatts.p_value = (uint8_t *) &(*pdata);
    return sd_ble_gatts_value_set(  m_bms.conn_handle,
                                    m_bms.bat_cellVoltage_char_handles.value_handle,
                                    &cellV_gatts);
}