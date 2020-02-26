// Stephen Donchez
// Dr. Wang
// ECE 8448-DL1
// 26 February 2020

/******************************************************************************
* pushbutton_ISR.c - provides the interrupt service routine for the KEY inputs
*					 for Homework Assignment 5, part 2. See README.md for 
*					 assignment description.
******************************************************************************/
#include "address_map_arm.h"	// for register names
#include "defines.h"			//for KEY defines

extern volatile int run;		// main.c

/******************************************************************************
* pushbutton_ISR: handles a key press in accordance with the assignment
*
* @param	N/A
* @returns	N/A
* @note		adding to pointers increments by number of registers, not # of bits
*			Key interrupt is cleared by writing any value to the Edge bits
*			register (KEY_ptr + 3)
*			The timer control register (HPS_TIMER0_ptr + 2) performs the
*			following functions:
*			Bit 0:		Timer enable (Active High)
*			Bit 1:		Load initial value from LOAD register (HPS_TIMER0_ptr)
*			Bit 2:		Interrupt enable (Active Low)
*			Bit 3-31:	Unused
* @see		DE1 SoC Computer Manual
******************************************************************************/
void pushbutton_ISR(void)
{
	volatile int* KEY_ptr = (int *) KEY_BASE;	// pushbutton KEY base address
	volatile int* HPS_TIMER0_ptr = (int*)HPS_TIMER0_BASE; //Timer base address

	int press = *(KEY_ptr + 3);					// read which key was pressed
	int timerStart = *(HPS_TIMER0_ptr);			// read current start value
	switch (press)
	{
		case KEY0:
			run = run ^ 1;						// toggle the value of run
			break;
		case KEY1:
			* (HPS_TIMER0_ptr + 2) = 0; 		// disable the timer

			// Load half the current value into the timer - doubles the speed
			*(HPS_TIMER0_ptr) = timerStart / 2;

			// enable interrupts for the timer
			// enable starting from the value in the LOAD register
			// enable the timer	
			*(HPS_TIMER0_ptr + 2) = 0b011;
			break;
		case KEY2:
			* (HPS_TIMER0_ptr + 2) = 0; 		// disable the timer

			// Load double the current value into the timer - halves the speed
			*(HPS_TIMER0_ptr) = timerStart * 2;

			// enable interrupts for the timer
			// enable starting from the value in the LOAD register
			// enable the timer	
			*(HPS_TIMER0_ptr + 2) = 0b011;
			break;
		default:
			break;
	} // Switch

	*(KEY_ptr + 3) = press;						// clear interrupt;
	return;
}//pushbutton_ISR