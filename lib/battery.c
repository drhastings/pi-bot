#include "../include/battery.h"

void initBattery()
{
  ADMUX |= 0<<ADLAR | 1<<MUX1 | 1<<MUX0;
  ADCSRA |= 1<<ADEN | 1<<ADIE | 1<<ADPS2 | 1<<ADPS1;
}

uint16_t readBattery()
{
  startConversion();
  return adc;
}

ISR (ADC_vect)
{
  adc = ADC;
}
