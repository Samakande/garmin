/*
 * helper.c
 *
 *  Created on: Oct 21, 2023
 *      Author: Student
 */

#include "helper.h"

float* start(float * samples, int sampling_rate, int baudrate) //detects when the dataline is pulled low by the transmitting device for one period
{
	float period = 1/baudrate;
	int bufferSize = sampling_rate/baudrate;
	float sum;
	float average;

	while (1) //checking incoming samples for a start condition
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
	}

}

void readByte(float * start, int sampling_rate, int baudrate)
{
	float period = 1/baudrate;
	int bufferSize = sampling_rate/baudrate;
	float sum;
	float average;
	char frame = 0x00;

	for(int i=0; i<8; i++)
	{

		for(int j=0; j<bufferSize; j++)
		{
			sum += *start++;
		}


		average = sum/bufferSize;

		(average > 1.6) ? (frame = frame || 1 << i ) : (frame = frame || 0 << i);

		//print character to console h

	}

}




