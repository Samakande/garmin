/*
 * helper.c
 *
 *  Created on: Oct 21, 2023
 *      Author: Student
 */

#include "helper.h"
#include <stddef.h>

float* start(float * samples, int sampling_rate, int baudrate, int arraySize) //detects when the dataline is pulled low by the transmitting device for one period
{
	float period = 1/baudrate;
	int bufferSize = sampling_rate/baudrate;
	float sum;
	float average;

	while (arraySize--) //checking incoming samples for a start condition
	{
		if(*samples < 0.9) //check if the current sample is less than 0.9v and start buffering the incoming samples for 1 period
		{
			for(int i=0; i<bufferSize; i++)
			{
				sum += *samples++;
			}

			average = sum/bufferSize;

			if(average < 0.9)
			{
				return samples; //returns a pointer to the start of the 1st bit being sampled
			}

		}
		samples++;
	}

	return NULL;

}

char readByte(float * start, int sampling_rate, int baudrate)
{
	//float period = 1/baudrate;
	//float rounding_error = sampling_rate%baudrate;

	int bufferSize;	//sampling_rate/baudrate;
	float sum;
	float average;
	char frame = 0x00;

	uint8_t sequence = 0b110;

	for(int i=0; i<8; i++)
	{

		(sequence > 3) ? (bufferSize =9) : (bufferSize =8); //mechanism to account for the rounding errors
		leftRotate(&sequence);
		sum=0;

		for(int j=0; j<bufferSize; j++)
		{
			sum += *start++;
		}


		average = sum/bufferSize;

		(average > 1.6) ? (frame |= (1 << 7-i)) : (frame |= (0 << 7-i)); //MSB first
	}

	//print character to console h
	return frame;

}

void leftRotate(uint8_t *num)
{
	uint8_t carry = *num & 100; // Get the highest bit
	*num <<= 1; // Shift the number to the left by 1
	*num |= carry>>2; // Set the lowest bit with the carry
	*num &= 0b00000111;
}



