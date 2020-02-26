// Stephen Donchez
// Dr. Wang
// ECE 8448-DL1
// 26 February 2020

/******************************************************************************
* timer0_ISR.c - provides the interrupt service routine for the HPS_TIMER0
*				 for Homework Assignment 5, part 2. See README.md for
*				 assignment description.
******************************************************************************/

#include "address_map_arm.h"	// for register names

extern volatile int run;		// main.c
extern volatile int count;		// main.c

/******************************************************************************
* timer0_ISR: handles a timer interrupt by incrementing the count and reading 
*			  the timer's end-of-interrupt register  to clear it
*
* @param	N/A
* @returns	N/A
* @note		adding to pointers increments by number of registers, not # of bits
*			Key interrupt is cleared by writing any value to the Edge bits
*			register (KEY_ptr + 3)
* @see		DE1 SoC Computer Manual
******************************************************************************/
void timer0_ISR(void)
{
	volatile int * HPS_TIMER0_ptr = (int *) HPS_TIMER0_BASE; // HPS timer addr.

	count += run;			// if we are running, increment. Otherwise don't

	*(HPS_TIMER0_ptr + 3);	// clear the interrupt
	return;
}// timer0_ISR