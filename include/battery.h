#ifndef BATTERY_H
#define BATTERY_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include "led.h"

#define startConversion() ADCSRA |= 1<< ADSC

volatile uint16_t adc;

void initBattery();

int16_t readBattery();

#endif
