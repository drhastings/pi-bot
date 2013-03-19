#ifndef ENCODERS_H
#define ENCODERS_H

#define LEFTENCODERA 13 //PB5
#define LEFTENCODERB 8 //PB0
#define RIGHTENCODERA 5 //PD5
#define RIGHTENCODERB 6 //PD6
// Left Encoder goes 0x00 -> 0x01 -> 0x21 -> 0x20 in forward direction.
// Right Encoder goes 0x00 -> 0x40 -> 0x60 -> 0x20 in forward direction.
#define LEFTENCODERMASK 0x21
#define RIGHTENCODERMASK 0x60

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/atomic.h>

typedef struct 
{
	uint8_t state;
	uint32_t position;
} encoder;

volatile encoder leftEncoder, rightEncoder;

void initEncoders();

int32_t getLeftCount();
int32_t getRightCount();

void processLeft();
void processRight();

#endif
