#include <wheel.h>

wheel leftWheel, rightWheel;

void initWheels()
{
	initMotors();
	initEncoders();
	makeWheel(&leftWheel,0,0,0,getLeftCount,motora);
	makeWheel(&rightWheel,0,0,0,getRightCount,motorb);
}

void makeWheel(wheel * myWheel, uint16_t P, uint16_t I, uint16_t D, int32_t (*read)(), void (*motor)(int16_t))
{
	myWheel->kP = P;
	myWheel->kI = I;
	myWheel->kD = D;
	myWheel->setPoint = 0;
	myWheel->setSpeed = 0;
	myWheel->speed = 0;
	myWheel->lastError = 0;
	myWheel->iError = 0;
	myWheel->lastError = 0;
	myWheel->encoderRead = read;
	myWheel->setMotor = motor;
	uint8_t x;
	for (x = 0; x < SPEED_BUFFER_DEPTH; x++) myWheel->speedBuff[x] = 0;
}

void getSpeed(wheel * myWheel)
{
	uint8_t x;

	for (x = SPEED_BUFFER_DEPTH - 1; x > 0; x--)
	{
		myWheel->speedBuff[x] = myWheel->speedBuff[x - 1];
	}
	myWheel->speedBuff[0] = myWheel->encoderRead();
	myWheel->speed = myWheel->speedBuff[0] - myWheel->speedBuff[SPEED_BUFFER_DEPTH - 1];
}

void PID(wheel * myWheel)
{
	int16_t pwm = 0;

	int16_t error = ((int16_t) myWheel->encoderRead()) - ((int16_t) (myWheel->setPoint>>8));

	pwm = myWheel->kP * error;

	pwm += myWheel->kD * (error - myWheel->lastError);
	myWheel->lastError = error;

	pwm += myWheel->kI * myWheel->iError;
	myWheel->iError += error;

	if (pwm > 255) pwm = 255;
	if (pwm < -255) pwm = -255;

	myWheel->setMotor(pwm);
	sendInt(pwm);
	sendLn();
}

void profile(wheel * myWheel)
{
	myWheel->setPoint += myWheel->setSpeed;
}

void processWheel(wheel * myWheel)
{
	getSpeed(myWheel);
	profile(myWheel);
	PID(myWheel);
}
