#include "../include/led.h"

void initSoftPWM()
{
  TCCR2A |= 1<<WGM21;
  TCCR2B |= 0<<CS22 | 1<<CS21 | 0<<CS20;
  OCR2A = 150;
  TIMSK2 |= 1<<OCIE2A;
  DDRD |= 0<<3 | 0<<4 | 0<<7;
}

void setRed(uint8_t intensity)
{
  red = intensity;
}

void setGreen(uint8_t intensity)
{
  green = intensity;
}

void setBlue(uint8_t intensity)
{
  blue = intensity;
}
ISR (TIMER2_COMPA_vect)
{
  uint8_t portmask = 0;
  portmask &= DDRD;
  if (red > counter)
  {
    portmask |= 1<<RED;
  }
  if (blue > counter)
  {
    portmask |= 1<<BLUE;
  }
  if (green > counter)
  {
    portmask |= 1<<GREEN;
  }
  counter++;
  DDRD = portmask;
}
