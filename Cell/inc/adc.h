#ifndef INC_ADC_H
#define INC_ADC_H

#include <avr/io.h>

#define ADC_CH0_PB5 0
#define ADC_CH1_PB2 1
#define ADC_CH2_PB4 2
#define ADC_CH3_PB3 3

//Calibration value for each device 1: -15

#define ADC_BANDGAP_mV_stock 1100
#define ADC_BANDGAP_mV_cal -15
#define ADC_MAXVALUE 1023UL
#define ADC_BANDGAP_mV (ADC_BANDGAP_mV_stock - ADC_BANDGAP_mV_cal)
#define ADC_MAGIC_NUM ( ADC_BANDGAP_mV * ADC_MAXVALUE )
// Optionally the internal 2.56V voltage reference may be decoupled by an external capacitor at the AREF pin to improve noise immunity.
void adc_init();
void adc_deinit();
uint16_t adc_getVcc();

#endif