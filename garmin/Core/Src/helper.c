/*
 * helper.c
 *
 *  Created on: Oct 21, 2023
 *      Author: Student
 */

#include "helper.h"
#include <stddef.h>

void start(decoding_handle * dhandle) //detects when the dataline is pulled low by the transmitting device for one period
{
	int bufferSize = (dhandle->sequence > 3) ? (bufferSize =9) : (bufferSize =8); //buffer size crafted to account for the sampling mismatch
	leftRotate(&(dhandle->sequence));
	float sum;
	float average;

	while (dhandle->arraySize--) //checking incoming samples for a start condition
	{
		if(*(dhandle->samplePointer) < 0.9) //check if the current sample is less than 0.9v and start buffering the incoming samples for 1 period. This helps to ensure the low voltage is not just a voltage spike
		{
			for(int i=0; i<bufferSize; i++)
			{
				if((i > 2) & (i <7))
				{
				sum += *(dhandle->samplePointer)++;
				/*keep track of the remaining samples*/
				dhandle->arraySize--;
				}

				else
				{
					(dhandle->samplePointer)++;
					dhandle->arraySize--;
				}
			}

			average = sum/4;

			if(average < 0.9)
			{
				dhandle->dstatus = STARTBIT_FOUND;
				break;
			}



		}

		else {(dhandle->samplePointer)++;}
	}

	if(dhandle->dstatus != STARTBIT_FOUND)
	{
		dhandle->dstatus = STARTBIT_NOTFOUND;
	}
}

void readByte(decoding_handle * dhandle)
{

	int bufferSize;	//sampling_rate/baudrate;
	float sum = 0;
	float average;
	char frame = 0x00;

	for(int i=0; i<8; i++) //bit sampling
	{
		(dhandle->sequence  > 3) ? (bufferSize =9) : (bufferSize =8);
		leftRotate(&(dhandle->sequence));

		for(int j=0; j<bufferSize; j++)
		{
			if((j > 2) & (j <7))
			{
			sum += *(dhandle->samplePointer)++;
			/*keep track of the remaining samples*/
			dhandle->arraySize--;
			}

			else
			{
				(dhandle->samplePointer)++;
				dhandle->arraySize--;
			}
		}

		/*voting system to decide whether the sampled bit is 0/1*/
		average = sum/4;
		(average > 1.6) ? (frame |= (1 << (7-i))) : (frame |= (0 << (7-i))); //MSB first

		/*reset sum for the next sample buffering*/
		sum=0;
	}

	dhandle->myChar = frame;
	dhandle->dstatus = FRAME_SUCCESS;

	//sample the stop bit : from the signal it looks like the uart is using 2 stop bits
	for(int k=0; k<2; k++)
	{
		(dhandle->sequence  > 3) ? (bufferSize =9) : (bufferSize =8);
		leftRotate(&(dhandle->sequence));

		for(int j=0; j<bufferSize; j++)
		{
			if((j > 2) & (j <7))
			{
			sum += *(dhandle->samplePointer)++;
			/*keep track of the remaining samples*/
			dhandle->arraySize--;
			}

			else
			{
				(dhandle->samplePointer)++;
				dhandle->arraySize--;
			}
		}

		if((sum/4) > 1.6) //if high pass
		{
			///
		}
		else
		{
			dhandle->dstatus = STOPBITS_NOTFOUND;
			break;
		}

	}

	//check the start bit for the next byte
	start(dhandle);

}

void leftRotate(uint8_t *num)
{
	uint8_t carry = *num & 100; // Get the highest bit
	*num <<= 1; // Shift the number to the left by 1
	*num |= carry>>2; // Set the lowest bit with the carry
	*num &= 0b00000111;
}



