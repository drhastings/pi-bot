#include <millis.h>
#include <encoders.h>

void initMillis()
{
  TCCR0A |= 1<<WGM01;
  TCCR0B |= 1<<CS02 | 0<<CS01 | 0<<CS00;
  OCR0A = 0x78;
  TIMSK0 |= 1<<OCIE0A;
	processMotors = 0;
}

uint32_t millis()
{
	return ms;
}

void delay(uint16_t wait)
{
  uint32_t time = millis();
  while (time + wait > millis());
}

ISR (TIMER0_COMPA_vect)
{
  ms++;
}
