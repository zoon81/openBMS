#include "adc.h"

void adc_init(){
    ADCSRA = (1 << ADEN) | (7 >> ADPS0); //128 prescaller
    ADMUX = 12; //Bandgap
}

uint16_t adc_getVcc(){
    ADCSRA |= (1 << ADSC);
    while( ! (ADCSRA & (1 << ADIF)));
    uint16_t ad_val = ADCH;
    ad_val <<= 8;
    ad_val |= ADCL;
<<<<<<< HEAD
    return ADC_MAGIC_NUM / ad_val;
=======
    return ad_val;
>>>>>>> 0af11edb2c3301e8d9ec7259e5c7c1e7211edd48
}