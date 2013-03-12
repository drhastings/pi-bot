#include <motors.h>
#include <avr/io.h>

void initMotors() 
{
  DDRC |= (1<<1) | (1<<2);
  DDRB |= (1<<1) | (1<<2) | (1<<3) | (1<<4);

  TCCR1A = (1<<COM1A1) | (1<<COM1B1);
  TCCR1B = (1<<WGM13) | (1<<CS12) | (0<<CS10);
  ICR1 = 255;
}

void motora(int16_t power)
{
  if (power < 0)
  {
    PORTB = (PORTB & 0xE7) | 0x08; 
    OCR1AL= -power;
  }
  else if (power > 0)
  {
    PORTB = (PORTB & 0xE7) | 0x10; 
    OCR1AL = power;
  }
  else
  {
    PORTB = (PORTB & 0xE7);
    OCR1A = 0;
  }
}

void motorb(int16_t power)
{
  if (power < 0)
  {
    PORTC = (PORTC & ~(0x06)) | 0x02; 
    OCR1B = -power;
  }
  else if (power > 0)
  {
    PORTC = (PORTC & ~(0x06)) | 0x04; 
    OCR1B = power;
  }
  else
  {
    PORTC = (PORTC & ~(0x06));
    OCR1B = 0;
  }
}
