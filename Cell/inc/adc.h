#ifndef INC_ADC_H
#define INC_ADC_H

#include <avr/io.h>

#define ADC_CH0_PB5 0
#define ADC_CH1_PB2 1
#define ADC_CH2_PB4 2
#define ADC_CH3_PB3 3

// Equation to get calibration value based on known Vcc value -->  calvalue = Vcc[mV] * adcValue / adcMaxValue - badgapVoltage[mV]
// Calibration value for each device 1: -15

#define ADC_BANDGAP_mV_stock 1100
#define ADC_BANDGAP_mV_cal 16
#define ADC_MAXVALUE 1023UL
#define ADC_BANDGAP_mV (ADC_BANDGAP_mV_stock + ADC_BANDGAP_mV_cal)
#define ADC_MAGIC_NUM ( ADC_BANDGAP_mV * ADC_MAXVALUE )
// Optionally the internal 2.56V voltage reference may be decoupled by an external capacitor at the AREF pin to improve noise immunity.

void adc_init();
void adc_deinit();
void adc_setCalibrationValue(int16_t calibration_value);
uint16_t adc_getRawVcc();
uint16_t adc_getAvgRawVcc(uint8_t samples);
uint16_t adc_getVcc();

#endif