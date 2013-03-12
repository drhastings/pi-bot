#include <encoders.h>

void initEncoders()
{
  leftEncoder.state = PINB & LEFTENCODERMASK;
	leftEncoder.position = 0;
	rightEncoder.state = PIND & RIGHTENCODERMASK;
	rightEncoder.position = 0;
	PCICR |= (1<<PCIE2) | (1<<PCIE0);
	PCMSK0 = LEFTENCODERMASK;
	PCMSK2 = RIGHTENCODERMASK;
}

int32_t getLeftCount()
{
	return leftEncoder.position;
}

int32_t getRightCount()
{
	return rightEncoder.position;
}

ISR (PCINT0_vect)
{
	uint8_t state = PINB & LEFTENCODERMASK;
	if (leftEncoder.state == 0x00)
	{
		if (state == 0x01)
		{
			leftEncoder.position++;
		}
		else
		{
			leftEncoder.position--;
		}
	}
	else if (leftEncoder.state == 0x01)
	{
		if (state == 0x21)
		{
			leftEncoder.position++;
		}
		else
		{
			leftEncoder.position--;
		}
	}
	else if (leftEncoder.state == 0x21)
	{
		if (state == 0x20)
		{
			leftEncoder.position++;
		}
		else
		{
			leftEncoder.position--;
		}
	}	
	else if (leftEncoder.state == 0x20)
	{
		if (state == 0x00)
		{
			leftEncoder.position++;
		}
		else
		{
			leftEncoder.position--;
		}
	}
	leftEncoder.state = state;
}

ISR (PCINT2_vect)
{
	uint8_t state = PIND & RIGHTENCODERMASK; 
	if (rightEncoder.state == 0x00)
	{
		if (state == 0x40)
		{
			rightEncoder.position++;
		}
		else
		{
			rightEncoder.position--;
		}
	}
	else if (rightEncoder.state == 0x40)
	{
		if (state == 0x60)
		{
			rightEncoder.position++;
		}
		else
		{
			rightEncoder.position--;
		}
	}
	else if (rightEncoder.state == 0x60)
	{
		if (state == 0x20)
		{
			rightEncoder.position++;
		}
		else
		{
			rightEncoder.position--;
		}
	}	
	else if (rightEncoder.state == 0x20)
	{
		if (state == 0x00)
		{
			rightEncoder.position++;
		}
		else
		{
			rightEncoder.position--;
		}
	}
	rightEncoder.state = state;
}
	
 
