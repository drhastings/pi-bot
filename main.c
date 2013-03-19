#include "main.h"

extern wheel leftWheel, rightWheel;

extern volatile uint8_t processMotors;

float P = -4000;
float I = 150;
float D = -5500;
float W = 10;
float Q = 25;
float lastError, iError;
float tilt = 0.010;
float mP = 450;
int8_t gainGuard, deadBand;
int16_t height = -90;
int16_t speed = 0;
float spin = 0;


float targetRotation = 0;
float rotation = 0;
float movement = 0;
int32_t position = 0;
int32_t lastLeft = 0;
int32_t lastRight = 0;
float lastPosError;

int16_t centerCount = 0;

int main()
{
	initBattery();
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
			processLeft();
			processRight();
			otherNow = millis();
		}
		if (now + 5 < millis())
		{
			if(!dmpGetQuaternion(&q, fifoBuffer))
			{
				dmpGetQuaternion(&q, fifoBuffer);
				dmpGetGravity(&gravity, & q);
				float angle = atan2(gravity.z, -gravity.y);
				float error = angle - tilt;
				float offset = 0;
				float pwm = 0;
				int8_t leftMovement = leftWheel.encoderRead() - lastLeft;
				int8_t rightMovement = rightWheel.encoderRead() - lastRight;
				cli();
				lastLeft = leftWheel.encoderRead();
				lastRight = rightWheel.encoderRead();
				sei();
				movement += ((leftMovement + rightMovement) / 2.0);
				rotation += (rightMovement - leftMovement) / 120.0;
				int16_t spinOffset = (rotation - targetRotation) * mP;
				float	pos = ((movement - (position>>8)) + (sin(error) * height));
				pwm += pos * W;
				getSpeed(&leftWheel);
				getSpeed(&rightWheel);
				pwm += Q * (leftWheel.speed + rightWheel.speed) / 2.0;
				if (!speed)
				{
					if (pos > 0) centerCount++;
					if (pos < 0) centerCount--;
					if (centerCount > 100)
					{
						centerCount = 0;
					//	tilt += 0.0001;
					}
					if (centerCount < -100)
					{
						centerCount = 0;
					//	tilt -= 0.0001;
					}
				}
				pwm += P * error;
				pwm += D * (error - lastError);
				lastError = error;
				pwm += I * iError;
				if(abs((iError + error) * I) < gainGuard) iError += error;

				if(pwm > 0) pwm += deadBand;
				if(pwm < 0) pwm -= deadBand;
				if(pwm > 255) pwm = 255;
				if(pwm < -255) pwm = -255;
				if(abs(angle) < 0.30 && now > 3000)
				{
					motora(pwm + spinOffset);
					motorb(pwm - spinOffset);
				}
				else
				{
					motora(0);
					motorb(0);
				}
			}
			position += speed;
			targetRotation += spin;
			char message[16];
			getLine(message);
			int16_t in;
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
					position = leftWheel.encoderRead()<<8;
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
				case 't':
					tilt = atof(message + 1);
					break;
				case 'h':
					position = movement;
					speed = 0;
					spin = 0;
					break;
				case 'm':
					mP = atof(message + 1);
					break;
				case 'v':
					speed = atoi(message + 1);
					if (speed > 200) speed = 200;
					if (speed < -200) speed = 200;
					break;
				case 's':
					in = atoi(message + 1);
					spin = in * 0.000007843;
					if (spin > 0.003) speed = 0.0003;
					if (spin < -0.0003) speed = -0.0003;
					break;
				default:
					break;
			}
			message[0] = 0;
			now = millis();
		}
	}
}
