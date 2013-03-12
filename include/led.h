#ifndef LED_H
#define LED_H

#include <avr/interrupt.h>
#include <avr/io.h>

#define RED 3
#define BLUE 4
#define GREEN 7

volatile uint8_t red, green, blue;
volatile uint8_t counter;

void initSoftPWM();

void setRed(uint8_t intensity);
void setGreen(uint8_t intensity);
void setBlue(uint8_t intensity);

#endif
