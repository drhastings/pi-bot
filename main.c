#include "main.h"

extern wheel leftWheel, rightWheel;

extern volatile uint8_t processMotors;

float P = -3200;
float I = 75;
float D = -3500;
float W = 4;
float Q = 25;
float lastError, iError;
int8_t gainGuard, deadBand;
int16_t height = 40;
int16_t speed = 0;
int16_t spin = 0;

int32_t position = 0;
float lastPosError;

int main()
{
	initMillis();
	initUART();
	initWheels();
	initMPU();
	sei();

	dmpInitialize();

	setDMPEnabled(1);

	now = millis();
	uint32_t otherNow = millis();
	
	leftWheel.kP = 0;
	leftWheel.kD = 0;

	Quaternion q;
	VectorFloat gravity;
	uint8_t fifoBuffer[42];

	while (1)
	{
		if(dmpPacketAvailable())
		{
			getFIFOBytes(fifoBuffer,42);
			
		}
		if (otherNow != millis())
		{
//			processWheel(&leftWheel);
//			processWheel(&rightWheel);
			otherNow = millis();
		}
		if (now + 5 < millis())
		{
			if(!dmpGetQuaternion(&q, fifoBuffer))
			{
				dmpGetQuaternion(&q, fifoBuffer);
				dmpGetGravity(&gravity, & q);
				float angle = atan2(gravity.z, -gravity.y);

				float pwm = P * angle;
				pwm += D * (angle - lastError);
				lastError = angle;
				pwm += I * iError;
				pwm += ((leftWheel.encoderRead() - (position>>8)) + (sin(angle) * height)) * W;
				getSpeed(&leftWheel);
				pwm += Q * leftWheel.speed;

				if(abs((iError + angle) * I) < gainGuard) iError += angle;

				if(pwm > 0) pwm += deadBand;
				if(pwm < 0) pwm -= deadBand;
				if(pwm > 255) pwm = 255;
				if(pwm < -255) pwm = -255;
				if(abs(angle) < 0.30)
				{
					motora(pwm + spin);
					motorb(pwm - spin);
				}
				else
				{
					motora(0);
					motorb(0);
				}
			}
			position += speed;
			char message[16];
			getLine(message);
			switch (message[0])
			{
				case 'p':
					P = atof(message + 1);
					break;
				case 'i':
					I = atof(message + 1);
					iError = 0;
					break;
				case 'd':
					D = atof(message + 1);
					break;
				case 'w':
					W = atof(message + 1);
					position = leftWheel.encoderRead();
					break;
				case 'q':
					Q = atof(message + 1);
					break;
				case 'g':
					gainGuard = atoi(message + 1);
					break;
				case 'b':
					deadBand = atoi(message + 1);
					break;
				case 'l':
					height = atoi(message + 1);
					break;
				case 'h':
					position = leftWheel.encoderRead();
					speed = 0;
					break;
				case 'v':
					speed = atoi(message + 1);
					break;
				case 's':
					spin = atoi(message + 1);
					break;
				default:
					break;
			}
			message[0] = 0;
			now = millis();
		}
	}
}
