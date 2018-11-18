#ifndef INC_INA219_H
#define INC_INA219_H

#include "twi.h"

#define INA219_ADDRESS_R 0x41
#define INA219_ADDRESS_W 0x40

#define INA_CONFIG_REG_ADDRESS 0x00
#define CFG_BUSVOLTAGE_RANGE_32V 1
#define CFG_BUSVOLTAGE_RANGE_16V 0

// PGA (Shunt Voltage Only)
#define CFG_PGA_40mV    0
#define CFG_PGA_80mV    1
#define CFG_PGA_160mV   2
#define CFG_PGA_320mV   3

// BADC Bus ADC Resolution/Averaging
#define CFG_BADC_RES_AVG_9bit   0
#define CFG_BADC_RES_AVG_10bit  1
#define CFG_BADC_RES_AVG_11bit  2
#define CFG_BADC_RES_AVG_12bit  3
#define CFG_BADC_RES_AVG_2samples 9
#define CFG_BADC_RES_AVG_4samples 10
#define CFG_BADC_RES_AVG_8samples 11
#define CFG_BADC_RES_AVG_16samples 12
#define CFG_BADC_RES_AVG_32samples 13
#define CFG_BADC_RES_AVG_64samples 14
#define CFG_BADC_RES_AVG_128samples 15

// SADC Shunt ADC Resolution/Averaging
#define CFG_SADC_RES_AVG_9bit   0
#define CFG_SADC_RES_AVG_10bit  1
#define CFG_SADC_RES_AVG_11bit  2
#define CFG_SADC_RES_AVG_12bit  3
#define CFG_SADC_RES_AVG_2samples 9
#define CFG_SADC_RES_AVG_4samples 10
#define CFG_SADC_RES_AVG_8samples 11
#define CFG_SADC_RES_AVG_16samples 12
#define CFG_SADC_RES_AVG_32samples 13
#define CFG_SADC_RES_AVG_64samples 14
#define CFG_SADC_RES_AVG_128samples 15

// Operating Mode
#define CFG_OPMODE_POWERDOWN                        0
#define CFG_OPMODE_SHUTVOLTAGE_TRIGERED             1
#define CFG_OPMODE_BUSVOLTAGE_TRIGERED              2
#define CFG_OPMODE_BUS_AND_SHUTVOLTAGE_TRIGERED     3
#define CFG_OPMODE_ADCOFF                           4
#define CFG_OPMODE_BUSVOLTAGE_CONT                  5
#define CFG_OPMODE_SHUTVOLTAGE_CONT                 6
#define CFG_OPMODE_BUS_AND_SHUTVOLTAGE_CONT         7

// Here you can set the Config register initial value
#define CFG_RESET_BIT                   0
#define CFG_BUSVOLTAGE_RANGE            CFG_BUSVOLTAGE_RANGE_32V
#define CFG_PGA                         CFG_PGA_320mV
#define CFG_BADC_RES_AVG                CFG_BADC_RES_AVG_12bit
#define CFG_SADC_RES_AVG                CFG_SADC_RES_AVG_12bit
#define CFG_OPMODE                      CFG_OPMODE_POWERDOWN

// Offsets for CONFIG reg.
#define CFG_RESET_OFFSET                15
#define CFG_BUSVOLTAGE_RANGE_OFFSET     13
#define CFG_PGA_OFFSET                  11
#define CFG_BADC_RES_AVG_OFFSET         7
#define CFG_SADC_RES_AVG_OFFSET         3
#define CFG_OPMODE_OFFSET               0

#define INA_CONFIG_REG_INIT_VALUE (CFG_RESET_BIT << CFG_RESET_OFFSET) | (CFG_BUSVOLTAGE_RANGE << CFG_BUSVOLTAGE_RANGE_OFFSET) | (CFG_PGA << CFG_PGA_OFFSET) | (CFG_BADC_RES_AVG << CFG_BADC_RES_AVG_OFFSET) | (CFG_SADC_RES_AVG << CFG_SADC_RES_AVG_OFFSET) | (CFG_OPMODE << CFG_OPMODE_OFFSET)

#endif