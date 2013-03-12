#ifndef WHEEL_H
#define WHEEL_H

#include <encoders.h>
#include <motors.h>
#include <uart.h>

#define SPEED_BUFFER_DEPTH 10

typedef struct
{
	int16_t kP;
	int16_t kI;
	int16_t kD;
	int32_t setPoint;
	int16_t setSpeed;
	int16_t speed;
	int16_t iError;
	int16_t lastError;
	int32_t (*encoderRead)();
	void (*setMotor) (int16_t);
	int16_t speedBuff[SPEED_BUFFER_DEPTH];
} wheel;

extern volatile encoder leftEncoder, rightEncoder;

void initWheels();

void makeWheel(wheel * myWheel, uint16_t P, uint16_t I, uint16_t D, int32_t (*read)(), void (*motor)(int16_t));

void getSpeed(wheel * myWheel);

void PID(wheel * myWheel);

void profile(wheel * myWheel);

void processWheel(wheel * myWheel);

#endif
