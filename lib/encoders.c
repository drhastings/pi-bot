#include <encoders.h>
#include <util/atomic.h>

void initEncoders()
{
  leftEncoder.state = PINB & LEFTENCODERMASK;
	leftEncoder.position = 0;
	rightEncoder.state = PIND & RIGHTENCODERMASK;
	rightEncoder.position = 0;
//	PCICR |= (1<<PCIE2) | (1<<PCIE0);
	PCMSK0 = LEFTENCODERMASK;
	PCMSK2 = RIGHTENCODERMASK;
}

int32_t getLeftCount()
{
	int32_t temp;
	cli();
	temp = leftEncoder.position;
	sei();
	return temp;
}

int32_t getRightCount()
{
	int32_t temp;
	cli();
	temp = rightEncoder.position;
	sei();
	return temp;
}

void processLeft()
{
	uint8_t state = PINB & LEFTENCODERMASK;
	if(state != leftEncoder.state)
	{
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
	}
	leftEncoder.state = state;
}

void processRight()
{
	uint8_t state = PIND & RIGHTENCODERMASK; 
	if (state != rightEncoder.state)
	{
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
	}
	rightEncoder.state = state;
}
	
 
