//Stephen Donchez
//Dr. Wang
//ECE 8448-DL1
//
5 February 2020
#include <stdio.h>
#include <stdbool.h>

/*****************************************************************************************
* Homework 3: reads the value from the set of toggle switches on the DE1-SoC and writes
*			  them to the corresponding LEDs.
*
* @returns void
* @note: memory addresses listed on lines 18 and 19 per Platform Designer
*****************************************************************************************/
void main()
{ 
	volatile int * LED = (int *) 0xFF200000; // Address of the PIO peripheral - LED
	volatile int * SW = (int *) 0xFF200040; // Address of the PIO peripheral - SW
	int curValue = 0x00;
	while( true ) 
	{
		// read switches
		curValue = *(SW);

		// control led
		*(LED) = curValue; 
	} // while
} // main
