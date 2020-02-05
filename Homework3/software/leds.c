#include <stdio.h>
#include <stdbool.h>
void main()
{ 
	volatile int * LED = (int *) 0xFF200000; // Address of the PIO peripheral - LED
	volatile int * SW = (int *) 0xFF200040; //Address of the PIO peripheral - SW
	int curValue = 0x00;
	while( true ) 
	{
		// read switches
		curValue = *(SW);
		// control led
		*(LED) = curValue; 
	} // while
}
