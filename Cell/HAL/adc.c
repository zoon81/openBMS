#include "adc.h"
#include "util/delay.h"
int16_t bandgap_calibration_value = 0;

void adc_init(){
    ADCSRA = (1 << ADEN) | (7 >> ADPS0); //128 prescaller
    ADMUX = 12; //Bandgap
}

void adc_deinit(){
    ADCSRA = 0;
    ADMUX = 0;
}

void adc_setCalibrationValue(int16_t calibration_value){
    bandgap_calibration_value = calibration_value;
}
uint16_t adc_getRawVcc(){
    uint16_t result;
    ADCSRA |= (1<<ADSC); // Convert
    while ( ADCSRA & (1 << ADSC));
    result = ADCW;
    return result; // Back-calculate AVcc in mV
}
uint16_t adc_getAvgRawVcc(uint8_t samples){
    if(samples > 64){
        samples = 64;
    }
    // dummy conversion
    adc_getRawVcc();
    uint16_t adcval_avg = 0;
    uint8_t index = 0;
    while(index++ < samples){
        adcval_avg += adc_getRawVcc();
    }
    return adcval_avg / samples;
}

uint16_t adc_getVcc(){
    uint16_t ad_val = adc_getAvgRawVcc(16);
    return ((uint32_t)(ADC_BANDGAP_mV_stock - bandgap_calibration_value)) * ADC_MAXVALUE / ad_val;
}