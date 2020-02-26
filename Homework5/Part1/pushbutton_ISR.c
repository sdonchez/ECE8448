// Stephen Donchez
// Dr. Wang
// ECE 8448-DL1
// 26 February 2020

/******************************************************************************
* pushbutton_ISR.c - provides the interrupt service routine for the KEY inputs
*					 for Homework Assignment 5, part 1. See README.md for 
*					 assignment description.
******************************************************************************/
#include "address_map_arm.h"	// for register names

extern volatile int run;		// main.c

/******************************************************************************
* pushbutton_ISR: handles a key press by toggling the value of run and clearing
*				  the key interrupt
*
* @param	N/A
* @returns	N/A
* @note		adding to pointers increments by number of registers, not # of bits
*			Key interrupt is cleared by writing any value to the Edge bits
*			register (KEY_ptr + 3)
* @see		DE1 SoC Computer Manual
******************************************************************************/
void pushbutton_ISR(void)
{
	volatile int* KEY_ptr = (int *) KEY_BASE;	// pushbutton KEY base address

	run = run ^ 1;								// toggle the value of run
	
	int press = *(KEY_ptr + 3);					// read which key was pressed
	*(KEY_ptr + 3) = press;						// clear interrupt;
	return;
}//pushbutton_ISR